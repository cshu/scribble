package griddoor.iiSwBasedSpInterfaces;

import java.io.IOException;
import java.net.UnknownHostException;
import java.util.logging.Level;

import griddoor.IIPropSets.SingleInstanceSense;
import griddoor.SwingBasedEditorUI.ButtonWithFixedText;
import griddoor.SwingBasedEditorUI.ButtonWithVal;
import griddoor.SwingBasedEditorUI.ComponentGroup;
import griddoor.SwingBasedEditorUI.ComponentGroupCanBeClosedWithEsc;
import griddoor.SwingBasedEditorUI.JavaStringTextArea;
import griddoor.SwingBasedEditorUI.TextTabPage;
import griddoor.SwingBasedEditorUI.UIContext;
import griddoor.util.UtilFuncs;

public class SingleArgVerbLineageEdit extends TextTabPage {

	public SingleArgVerbLineageEdit(UIContext context, long id) throws UnknownHostException, IOException{
		super(context);
		components.add(makeVerbCom(id));
	}

	//undone new feature: given 2 verbs, try to connect them with lineage. if they have no entailment relation, show the failure message. if there is 1 or more paths, let user choose a path
	
	JavaStringTextArea makeVerbCom(final long vid) throws UnknownHostException, IOException{
		Globals.getOuStre().write(Globals.SELECT_SAVERB);
		UtilFuncs.outputStreamSendInt64(Globals.getOuStre(), vid);
		final String linetoshow = SingleInstanceSense.recvSAVerb(vid).gets(0)+'\n';
		return new JavaStringTextArea(this){
			@Override
			public long getl() {
				return vid;
			}
			@Override
			public void fillByPrepending() {
				fillEditorWithLines(linetoshow);
			}
			@Override
			public void bindStrFunc() {
				final String temstr = cg.context.editor.textInput.getText();
				if(temstr.isEmpty())
					return;
				ComponentGroup newcg = new ComponentGroup(this.cg);
				try {
					Globals.getOuStre().write(Globals.SEARCH_VERBDESC);
					UtilFuncs.outputStreamSendStrCriterionNEscChar(Globals.getOuStre(), temstr);
					Globals.getOuStre().write(0);
					boolean anyVerb = false;
					do{
						SingleInstanceSense vsens = SingleInstanceSense.recvVerb();
						if(vsens == null)
							break;
						anyVerb = true;
						newcg.components.add(new ButtonWithVal<SingleInstanceSense>(newcg,vsens){
							@Override
							public void spacePressed() {
								final long selectedid = this.val.id;
								this.cg.components.clear();
								this.cg.components.add(new ButtonWithFixedText(this.cg, "Insert above (entailed by this)"){
									@Override
									public void spacePressed() {
										try {
											Globals.getOuStre().write(Globals.INSERT_ENTAILMENT);
											UtilFuncs.outputStreamSendInt64(Globals.getOuStre(), vid);
											UtilFuncs.outputStreamSendInt64(Globals.getOuStre(), selectedid);
											if(Globals.getInStre().read()!=0){
												showMsgBox("Insertion failed");
												this.cg.context.redrawNeeded = true;
												return;
											}
											this.cg.cg.components.subList(0, this.cg.cg.cursorIndex).clear();
											this.cg.cg.components.add(0, makeVerbCom(selectedid));
										} catch (IOException e) {
											Globals.getLogger().log(Level.SEVERE, e.getMessage(), e);
											showMsgBox("Exception thrown when trying to insert");
											this.cg.context.redrawNeeded = true;
											return;
										}
										this.cg.closeItself();
										this.cg.context.redrawNeeded = true;
									}
								});
								this.cg.components.add(new ButtonWithFixedText(this.cg, "Insert below (entailing this)"){
									@Override
									public void spacePressed() {
										try {
											Globals.getOuStre().write(Globals.INSERT_ENTAILMENT);
											UtilFuncs.outputStreamSendInt64(Globals.getOuStre(), selectedid);
											UtilFuncs.outputStreamSendInt64(Globals.getOuStre(), vid);
											if(Globals.getInStre().read()!=0){
												showMsgBox("Insertion failed");
												this.cg.context.redrawNeeded = true;
												return;
											}
											this.cg.cg.components.subList(this.cg.cg.cursorIndex+1,this.cg.cg.components.size()).clear();
											this.cg.cg.components.add(makeVerbCom(selectedid));
										} catch (IOException e) {
											Globals.getLogger().log(Level.SEVERE, e.getMessage(), e);
											showMsgBox("Exception thrown when trying to insert");
											this.cg.context.redrawNeeded = true;
											return;
										}
										this.cg.closeItself();
										this.cg.context.redrawNeeded = true;
									}
								});
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
				this.cg.modalD = new ComponentGroupCanBeClosedWithEsc(this.cg);
				this.cg.modalD.components.add(new ButtonWithFixedText(this.cg.modalD, "Select verbs directly entailed by this one"){
					@Override
					public void spacePressed() {
						ComponentGroup newcg = new ComponentGroup(this.cg.cg);
						try {
							Globals.getOuStre().write(Globals.SELECTVERBSDIRECTLYENTAILEDBYTHIS);
							UtilFuncs.outputStreamSendInt64(Globals.getOuStre(), vid);
							boolean anyVerb = false;
							do{
								SingleInstanceSense vsens = SingleInstanceSense.recvVerb();
								if(vsens == null)
									break;
								anyVerb = true;
								newcg.components.add(new ButtonWithVal<SingleInstanceSense>(newcg,vsens){
									@Override
									public void spacePressed() {
										this.cg.cg.components.subList(0, this.cg.cg.cursorIndex).clear();
										try {
											this.cg.cg.components.add(0,makeVerbCom(this.val.id));
										} catch (IOException e) {
											Globals.getLogger().log(Level.SEVERE, e.getMessage(), e);
											showMsgBox("Exception thrown when trying to select");
											this.cg.context.redrawNeeded = true;
											return;
										}
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
				this.cg.modalD.components.add(new ButtonWithFixedText(this.cg.modalD, "Select verbs directly entailing this one"){
					@Override
					public void spacePressed() {
						ComponentGroup newcg = new ComponentGroup(this.cg.cg);
						try {
							Globals.getOuStre().write(Globals.SELECTVERBSDIRECTLYENTAILINGTHIS);
							UtilFuncs.outputStreamSendInt64(Globals.getOuStre(), vid);
							boolean anyVerb = false;
							do{
								SingleInstanceSense vsens = SingleInstanceSense.recvVerb();
								if(vsens == null)
									break;
								anyVerb = true;
								newcg.components.add(new ButtonWithVal<SingleInstanceSense>(newcg,vsens){
									@Override
									public void spacePressed() {
										this.cg.cg.components.subList(this.cg.cg.cursorIndex+1,this.cg.cg.components.size()).clear();
										try {
											this.cg.cg.components.add(makeVerbCom(this.val.id));
										} catch (IOException e) {
											Globals.getLogger().log(Level.SEVERE, e.getMessage(), e);
											showMsgBox("Exception thrown when trying to select");
											this.cg.context.redrawNeeded = true;
											return;
										}
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
				this.cg.modalD.components.add(new ButtonWithFixedText(this.cg.modalD, "Open edit page of verb"){
					@Override
					public void spacePressed() {
						try {
							this.cg.context.editor.addTabNSwitchToIt(new SingleArgVerbEdit(this.cg.context,vid));
						} catch (IOException e) {
							Globals.getLogger().log(Level.SEVERE, e.getMessage(), e);
							showMsgBox("Exception thrown when trying to open SingleArgVerbEdit page");
							this.cg.context.redrawNeeded = true;
						}
					}
				});
				this.cg.context.redrawNeeded = true;
			}
			@Override
			public void xPressed() {
				this.cg.modalD = new ComponentGroupCanBeClosedWithEsc(this.cg);
				this.cg.modalD.components.add(new ButtonWithFixedText(this.cg.modalD, "Remove this one and all ABOVE from view"){
					@Override
					public void spacePressed() {
						if(this.cg.cg.cursorIndex+1==this.cg.cg.components.size())
							return;
						this.cg.cg.components.subList(0, this.cg.cg.cursorIndex+1).clear();
						this.cg.closeItself();
						this.cg.context.redrawNeeded = true;
					}
				});
				this.cg.modalD.components.add(new ButtonWithFixedText(this.cg.modalD, "Remove this one and all BELOW from view"){
					@Override
					public void spacePressed() {
						if(this.cg.cg.cursorIndex==0)
							return;
						this.cg.cg.components.subList(this.cg.cg.cursorIndex, this.cg.cg.components.size()).clear();
						this.cg.closeItself();
						this.cg.context.redrawNeeded = true;
					}
				});
				this.cg.modalD.components.add(new ButtonWithFixedText(this.cg.modalD, "Remove this one and all ABOVE (update DB to break with the verb below)"){
					@Override
					public void spacePressed() {
						if(this.cg.cg.cursorIndex+1==this.cg.cg.components.size())
							return;
						try {
							Globals.getOuStre().write(Globals.DELETE_ENTAILMENT);
							UtilFuncs.outputStreamSendInt64(Globals.getOuStre(), this.cg.cg.components.get(this.cg.cg.cursorIndex+1).getl());
							UtilFuncs.outputStreamSendInt64(Globals.getOuStre(), vid);
							if(Globals.getInStre().read()!=0){
								showMsgBox("Deletion failed");
								this.cg.context.redrawNeeded = true;
								return;
							}
						} catch (IOException e) {
							Globals.getLogger().log(Level.SEVERE, e.getMessage(), e);
							showMsgBox("Exception thrown when trying to delete");
							this.cg.context.redrawNeeded = true;
							return;
						}
						this.cg.cg.components.subList(0, this.cg.cg.cursorIndex+1).clear();
						this.cg.closeItself();
						this.cg.context.redrawNeeded = true;
					}
				});
				this.cg.modalD.components.add(new ButtonWithFixedText(this.cg.modalD, "Remove this one and all BELOW (update DB to break with the verb above)"){
					@Override
					public void spacePressed() {
						if(this.cg.cg.cursorIndex==0)
							return;
						try {
							Globals.getOuStre().write(Globals.DELETE_ENTAILMENT);
							UtilFuncs.outputStreamSendInt64(Globals.getOuStre(), vid);
							UtilFuncs.outputStreamSendInt64(Globals.getOuStre(), this.cg.cg.components.get(this.cg.cg.cursorIndex-1).getl());
							if(Globals.getInStre().read()!=0){
								showMsgBox("Deletion failed");
								this.cg.context.redrawNeeded = true;
								return;
							}
						} catch (IOException e) {
							Globals.getLogger().log(Level.SEVERE, e.getMessage(), e);
							showMsgBox("Exception thrown when trying to delete");
							this.cg.context.redrawNeeded = true;
							return;
						}
						this.cg.cg.components.subList(this.cg.cg.cursorIndex, this.cg.cg.components.size()).clear();
						this.cg.closeItself();
						this.cg.context.redrawNeeded = true;
					}
				});
				this.cg.context.redrawNeeded = true;
			}
		};
	}
}
