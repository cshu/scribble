package griddoor.iiSwBasedSpInterfaces;

import griddoor.IIPropSets.SingleInstanceSense;
import griddoor.SwingBasedEditorUI.ButtonWithFixedText;
import griddoor.SwingBasedEditorUI.ButtonWithVal;
import griddoor.SwingBasedEditorUI.ComponentGroup;
import griddoor.SwingBasedEditorUI.ComponentGroupCanBeClosedWithEsc;
import griddoor.SwingBasedEditorUI.JStrTextAreaWithLabel;
import griddoor.SwingBasedEditorUI.TextComponent;
import griddoor.SwingBasedEditorUI.TextTabPage;
import griddoor.SwingBasedEditorUI.UIContext;
import griddoor.util.UtilFuncs;

import java.io.IOException;
import java.net.UnknownHostException;
import java.util.logging.Level;

public class SingleArgVerbMutexGroupEdit extends TextTabPage {
	
	long snId;

	public SingleArgVerbMutexGroupEdit(UIContext context, long id) throws UnknownHostException, IOException{
		super(context);
		snId = id;
		Globals.getOuStre().write(Globals.SELECT_MUTEXGROUP);
		UtilFuncs.outputStreamSendInt64(Globals.getOuStre(), snId);
		boolean anyVerb = false;
		do{
			SingleInstanceSense vsens = SingleInstanceSense.recvVerb();
			if(vsens == null)
				break;
			anyVerb = true;
			components.add(makeBtnOfV(vsens));
		}while(true);
		if(!anyVerb){
			components.add(new ButtonWithFixedText(this,"Group doesn't exist, CLOSE THE TAB"){
				@Override
				public void spacePressed() {
					this.cg.context.editor.tabs.removeLast();
					this.cg.context.editor.currentTab = this.cg.context.editor.tabs.getLast();
					this.cg.context.redrawNeeded = true;
				}
			});
			return;
		}
		addComs();
	}
	
	SingleArgVerbMutexGroupEdit(UIContext context){
		super(context);
	}
	
	void addComs(){
		components.add(new JStrTextAreaWithLabel(this, "Add verb"){
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
							@Override
							public void spacePressed() {
								this.cg.cg.components.add(this.cg.cg.cursorIndex, makeBtnOfV(this.val));
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
		});
		components.add(new ButtonWithFixedText(this, "SAVE"){
			@Override
			public void spacePressed() {
				if(this.cg.components.size()<5){
					showMsgBox("Cannot save. At least 2 verbs are needed.");
					this.cg.context.redrawNeeded = true;
					return;
				}
				try {
					Globals.getOuStre().write(Globals.UPSERT_MUTEX);
					int countoftc = this.cg.components.size()-3;
					for(TextComponent tc:this.cg.components){
						UtilFuncs.outputStreamSendInt64(Globals.getOuStre(), tc.getl());
						countoftc--;
						if(countoftc==0){
							Globals.getOuStre().write(0);
							break;
						}else{
							Globals.getOuStre().write(1);
						}
					}
					UtilFuncs.outputStreamSendInt64(Globals.getOuStre(), snId);
					int retcod = Globals.getInStre().read();
					if(retcod!=0){
						showMsgBox("Saving failed with code:"+retcod);
						this.cg.context.redrawNeeded = true;
						return;
					}
					snId=UtilFuncs.inputStreamRecvInt64(Globals.getInStre());
				} catch (IOException e) {
					Globals.getLogger().log(Level.SEVERE, e.getMessage(), e);
					showMsgBox("Exception thrown when trying to save");
					this.cg.context.redrawNeeded = true;
					return;
				}
				showMsgBox("Saved successfully!");
				this.cg.context.redrawNeeded = true;
			}
		});
		components.add(new ButtonWithFixedText(this, "DELETE this group"){
			@Override
			public void spacePressed() {
				if(snId==0){
					showMsgBox("This is a new group. You cannot delete it.");
				}else{
					this.cg.modalD = new ComponentGroupCanBeClosedWithEsc(this.cg);
					this.cg.modalD.components.add(new ButtonWithFixedText(this.cg.modalD, "Confirm the deletion"){
						@Override
						public void spacePressed() {
							try {
								Globals.getOuStre().write(Globals.DELETE_MUTEX);
								UtilFuncs.outputStreamSendInt64(Globals.getOuStre(), snId);
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
							this.cg.context.editor.tabs.removeLast();
							this.cg.context.editor.currentTab = this.cg.context.editor.tabs.getLast();
							this.cg.context.redrawNeeded = true;
						}
					});
				}
				this.cg.context.redrawNeeded = true;
			}
		});
	}
	
	ButtonWithVal<SingleInstanceSense> makeBtnOfV(SingleInstanceSense vsens){
		return new ButtonWithVal<SingleInstanceSense>(this,vsens){
			@Override
			public long getl() {
				return this.val.id;
			}
			@Override
			public void xPressed() {
				this.cg.components.remove(this.cg.cursorIndex);
				this.cg.context.redrawNeeded = true;
			}
			@Override
			public void spacePressed() {
				try {
					this.cg.context.editor.addTabNSwitchToIt(new SingleArgVerbEdit(this.cg.context,this.val.id));
				} catch (IOException e) {
					Globals.getLogger().log(Level.SEVERE, e.getMessage(), e);
					showMsgBox("Exception thrown when trying to open SingleArgVerbEdit page");
					this.cg.context.redrawNeeded = true;
				}
			}
		};
	}
	
	public static SingleArgVerbMutexGroupEdit makeNewGroup(UIContext context, long veid) throws UnknownHostException, IOException{
		SingleArgVerbMutexGroupEdit valtoret = new SingleArgVerbMutexGroupEdit(context);
		Globals.getOuStre().write(Globals.SELECT_SAVERB);
		UtilFuncs.outputStreamSendInt64(Globals.getOuStre(), veid);
		valtoret.components.add(valtoret.makeBtnOfV(SingleInstanceSense.recvSAVerb(veid)));
		valtoret.addComs();
		return valtoret;
	}
}
