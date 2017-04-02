package griddoor.iiSwBasedSpInterfaces;

import java.io.IOException;
import java.net.UnknownHostException;
import java.util.logging.Level;

import griddoor.IIPropSets.SingleInstanceSense;
import griddoor.SwingBasedEditorUI.ButtonWithFixedText;
import griddoor.SwingBasedEditorUI.ButtonWithVal;
import griddoor.SwingBasedEditorUI.ComponentGroup;
import griddoor.SwingBasedEditorUI.ComponentGroupCanBeClosedWithEsc;
import griddoor.SwingBasedEditorUI.JStrTextAreaWithLabel;
import griddoor.SwingBasedEditorUI.TextTabPage;
import griddoor.SwingBasedEditorUI.UIContext;
import griddoor.util.UtilFuncs;

public class SingleArgPropsEdit extends TextTabPage {

	public long snId;//short for sense id
	public String firstLineDesc;
	
	public SingleArgPropsEdit(UIContext context, long snId, String firstLineDesc) throws UnknownHostException, IOException {
		super(context);
		this.snId = snId;
		this.firstLineDesc = firstLineDesc;
		components.add(new ButtonWithFixedText(this,firstLineDesc){
			@Override
			public void spacePressed() {
				this.cg.modalD = new ComponentGroupCanBeClosedWithEsc(this.cg);
				this.cg.modalD.components.add(new ButtonWithFixedText(this.cg.modalD, "Refresh and discard all changes"){
					@Override
					public void spacePressed() {
						try {
							this.cg.context.editor.currentTab = new SingleArgPropsEdit(context,snId,firstLineDesc);
						} catch (IOException e) {
							Globals.getLogger().log(Level.SEVERE, e.getMessage(), e);
							showMsgBox("Refreshing failed.");
							this.cg.context.redrawNeeded = true;
							return;
						}
						this.cg.context.editor.tabs.set(this.cg.context.editor.tabs.size()-1, this.cg.context.editor.currentTab);
						context.redrawNeeded = true;
					}
				});
				this.cg.context.redrawNeeded = true;
			}
		});
		
		//undone change this button to JStrAreaWithLabel, "Applicable general verbs/Bind to search applicable verbs"
		components.add(new JStrTextAreaWithLabel(this,"Applicable general verbs/Bind to search verbs"){
			@Override
			public void bindStrFunc() {
				super.bindStrFunc();
				if(javaStr.isEmpty())
					return;
				ComponentGroup newcg = new ComponentGroup(this.cg);
				try {
					Globals.getOuStre().write(Globals.SEARCH_VERBDESC);
					UtilFuncs.outputStreamSendStrCriterionNEscChar(Globals.getOuStre(), javaStr);
					Globals.getOuStre().write(0);
					boolean anyVerb = false;
					do{
						SingleInstanceSense vsens = SingleInstanceSense.recvVerb();
						if(vsens == null)
							break;
						anyVerb = true;
						newcg.components.add(new ButtonWithVal<SingleInstanceSense>(newcg,vsens){
							public void spacePressed() {
								try {
									Globals.getOuStre().write(Globals.CHKPROPSVALIDITY);
									Globals.getOuStre().write(1);
									UtilFuncs.outputStreamSendInt64(Globals.getOuStre(),this.val.id);
									sendAllVerbs();
									int cod = Globals.getInStre().read();
									if(cod!=0){
										showMsgBox("Verb is not applicable with error code: "+Globals.getErrorMsgFromCod(cod));
										this.cg.context.redrawNeeded = true;
										return;
									}
								} catch (IOException e) {
									Globals.getLogger().log(Level.SEVERE, e.getMessage(), e);
									showMsgBox("Exception thrown when trying to check applicability");
									this.cg.context.redrawNeeded = true;
									return;
								}
								this.cg.cg.components.add(this.cg.cg.components.size()-1, makeButtonOfAppliedVerb(this.val));
								this.cg.closeItself();
								this.cg.context.redrawNeeded = true;
							}
						});
					}while(true);
					if(!anyVerb){
						showMsgBox("No verb found");
						this.cg.context.redrawNeeded = true;
						return;
					}
				} catch (IOException e) {
					Globals.getLogger().log(Level.SEVERE, e.getMessage(), e);
					showMsgBox("Exception thrown when trying to search verb desc");
					this.cg.context.redrawNeeded = true;
					return;
				}
				this.cg.modalD = newcg;
				this.cg.context.redrawNeeded = true;
			}
			@Override
			public void spacePressed() {
				ComponentGroupCanBeClosedWithEsc newcg = new ComponentGroupCanBeClosedWithEsc(this.cg);
				//undone first com is javastringtextarea as filter
				try {
					Globals.getOuStre().write(Globals.APPLICABLEGENERALVERBS);
					sendAllVerbs();
					int cod = Globals.getInStre().read();
					if(cod!=0){
						showMsgBox("'Applicable general verbs' failed with error code: "+cod);
						this.cg.context.redrawNeeded = true;
						return;
					}
					boolean anyVerb = false;
					do{
						SingleInstanceSense vsens = SingleInstanceSense.recvVerb();
						if(vsens == null)
							break;
						anyVerb = true;
						newcg.components.add(new ButtonWithVal<SingleInstanceSense>(newcg,vsens){
							@Override
							public void spacePressed() {
								//undone show high level verbs, each expand to low level children when pressed
								this.cg.cg.components.add(this.cg.cg.components.size()-1, makeButtonOfAppliedVerb(this.val));
								this.cg.closeItself();
								this.cg.context.redrawNeeded = true;
							};
						});
					}while(true);
					if(!anyVerb){
						showMsgBox("No verb found");
						this.cg.context.redrawNeeded = true;
						return;
					}
				} catch (IOException e) {
					Globals.getLogger().log(Level.SEVERE, e.getMessage(), e);
					showMsgBox("Requesting verb list failed.");
					this.cg.context.redrawNeeded = true;
					return;
				}
				this.cg.modalD = newcg;
				this.cg.context.redrawNeeded = true;
			}
		});
		components.add(new ButtonWithFixedText(this, "View all implicit verbs"));//undone
		Globals.getOuStre().write(Globals.SELECT_SAPROP);
		UtilFuncs.outputStreamSendInt64(Globals.getOuStre(), snId);
		do{
			SingleInstanceSense vsens = SingleInstanceSense.recvVerb();
			if(vsens == null)
				break;
			components.add(makeButtonOfAppliedVerb(vsens));
		}while(true);
		components.add(new ButtonWithFixedText(this, "SAVE"){
			@Override
			public void spacePressed() {
				try {
					Globals.getOuStre().write(Globals.SAVESAPROPS);
					UtilFuncs.outputStreamSendInt64(Globals.getOuStre(),snId);
					sendAllVerbs();
					int cod = Globals.getInStre().read();
					if(cod==0){
						showMsgBox("Saved successfully!");
					}else{
						showMsgBox("Saving failed with error code: "+cod);
					}
				} catch (IOException e) {
					Globals.getLogger().log(Level.SEVERE, e.getMessage(), e);
					showMsgBox("Saving failed!");
				}
				this.cg.context.redrawNeeded = true;
			}
		});
	}
	
	ButtonWithVal<SingleInstanceSense> makeButtonOfAppliedVerb(SingleInstanceSense vsens){
		return new ButtonWithVal<SingleInstanceSense>(this,vsens){
			@Override
			public long getl() {
				return this.val.id;
			}
			@Override
			public void xPressed() {
				final long thisid = this.val.id;
				this.cg.modalD = new ComponentGroupCanBeClosedWithEsc(this.cg);
				this.cg.modalD.components.add(new ButtonWithFixedText(this.cg.modalD, "Detach this verb"){
					@Override
					public void spacePressed() {
						this.cg.cg.components.remove(this.cg.cg.cursorIndex);
						this.cg.closeItself();
						this.cg.context.redrawNeeded = true;
					}
				});
				this.cg.modalD.components.add(new ButtonWithFixedText(this.cg.modalD, "Replace with a verb entailed by this one"){
					@Override
					public void spacePressed() {
						ComponentGroup newcg = new ComponentGroup(this.cg.cg);
						try {
							Globals.getOuStre().write(Globals.SELECTVERBSDIRECTLYENTAILEDBYTHIS);
							UtilFuncs.outputStreamSendInt64(Globals.getOuStre(), thisid);
							boolean anyVerb = false;
							do{
								SingleInstanceSense vsens = SingleInstanceSense.recvVerb();
								if(vsens == null)
									break;
								anyVerb = true;
								newcg.components.add(new ButtonWithVal<SingleInstanceSense>(newcg,vsens){
									@Override
									public void spacePressed() {
										try {
											Globals.getOuStre().write(Globals.CHKPROPSVALIDITY);
											Globals.getOuStre().write(1);
											UtilFuncs.outputStreamSendInt64(Globals.getOuStre(),this.val.id);
											sendAllVerbs(this.cg.cg.cursorIndex);
											int cod = Globals.getInStre().read();
											if(cod!=0){
												showMsgBox("Verb is not applicable with error code: "+Globals.getErrorMsgFromCod(cod));
												this.cg.context.redrawNeeded = true;
												return;
											}
										} catch (IOException e) {
											Globals.getLogger().log(Level.SEVERE, e.getMessage(), e);
											showMsgBox("Exception thrown when trying to check applicability");
											this.cg.context.redrawNeeded = true;
											return;
										}
										this.cg.cg.components.set(this.cg.cg.cursorIndex, makeButtonOfAppliedVerb(this.val));
										this.cg.closeItself();
										this.cg.context.redrawNeeded = true;
									}
								});
							}while(true);
							if(!anyVerb){
								showMsgBox("No verb found");
								this.cg.context.redrawNeeded = true;
								return;
							}
						} catch (IOException e) {
							Globals.getLogger().log(Level.SEVERE, e.getMessage(), e);
							showMsgBox("Exception thrown when trying to select verbs");
							this.cg.context.redrawNeeded = true;
							return;
						}
						newcg.sitOnModalD();
						this.cg.context.redrawNeeded = true;
					}
				});
				this.cg.context.redrawNeeded = true;
			}
			@Override
			public void spacePressed() {
				ComponentGroup newcg = new ComponentGroup(this.cg);
				try {
					Globals.getOuStre().write(Globals.SELECTVERBSDIRECTLYENTAILINGTHIS);
					UtilFuncs.outputStreamSendInt64(Globals.getOuStre(), this.val.id);
					boolean anyVerb = false;
					do{
						SingleInstanceSense vsens = SingleInstanceSense.recvVerb();
						if(vsens == null)
							break;
						anyVerb = true;
						newcg.components.add(new ButtonWithVal<SingleInstanceSense>(newcg,vsens){
							@Override
							public void spacePressed() {
								try {
									Globals.getOuStre().write(Globals.CHKPROPSVALIDITY);
									Globals.getOuStre().write(1);
									UtilFuncs.outputStreamSendInt64(Globals.getOuStre(),this.val.id);
									sendAllVerbs(this.cg.cg.cursorIndex);
									int cod = Globals.getInStre().read();
									if(cod!=0){
										showMsgBox("Verb is not applicable with error code: "+Globals.getErrorMsgFromCod(cod));
										this.cg.context.redrawNeeded = true;
										return;
									}
								} catch (IOException e) {
									Globals.getLogger().log(Level.SEVERE, e.getMessage(), e);
									showMsgBox("Exception thrown when trying to check applicability");
									this.cg.context.redrawNeeded = true;
									return;
								}
								this.cg.cg.components.set(this.cg.cg.cursorIndex, makeButtonOfAppliedVerb(this.val));
								this.cg.closeItself();
								this.cg.context.redrawNeeded = true;
							}
						});
					}while(true);
					if(!anyVerb){
						showMsgBox("No verb found");
						this.cg.context.redrawNeeded = true;
						return;
					}
				} catch (IOException e) {
					Globals.getLogger().log(Level.SEVERE, e.getMessage(), e);
					showMsgBox("Exception thrown when trying to select verbs");
					this.cg.context.redrawNeeded = true;
					return;
				}
				this.cg.modalD = newcg;
				this.cg.context.redrawNeeded = true;
			}
		};
	}
	
	void sendAllVerbs() throws UnknownHostException, IOException{
		for(int i=3,comlastind=this.components.size()-1;i<comlastind;i++){
			Globals.getOuStre().write(1);
			UtilFuncs.outputStreamSendInt64(Globals.getOuStre(),((SingleInstanceSense)((ButtonWithVal<?>)this.components.get(i)).val).id);
		}
		Globals.getOuStre().write(0);
	}
	void sendAllVerbs(int excludeind) throws UnknownHostException, IOException{
		for(int i=3,comlastind=this.components.size()-1;i<comlastind;i++){
			if(excludeind==i)
				continue;
			Globals.getOuStre().write(1);
			UtilFuncs.outputStreamSendInt64(Globals.getOuStre(),((SingleInstanceSense)((ButtonWithVal<?>)this.components.get(i)).val).id);
		}
		Globals.getOuStre().write(0);
	}

	@Override
	public String toString() {
		return "[SA Propositions Edit] "+firstLineDesc;
	}
}
