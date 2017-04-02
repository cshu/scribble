package griddoor.iiSwBasedSpInterfaces;

import java.io.IOException;
import java.net.UnknownHostException;
import java.util.logging.Level;

import griddoor.IIPropSets.SingleInstanceSense;
import griddoor.SwingBasedEditorUI.ButtonWithFixedText;
import griddoor.SwingBasedEditorUI.ButtonWithStriVal;
import griddoor.SwingBasedEditorUI.ComponentGroup;
import griddoor.SwingBasedEditorUI.ComponentGroupCanBeClosedWithEsc;
import griddoor.SwingBasedEditorUI.JStrTextAreaWithSynopsis;
import griddoor.SwingBasedEditorUI.TextTabPage;
import griddoor.SwingBasedEditorUI.UIContext;
import griddoor.util.UtilFuncs;

public class SingleArgVerbEdit extends TextTabPage {

	public long snId;
	
	public SingleArgVerbEdit(UIContext context, long id) throws UnknownHostException, IOException {
		super(context);
		snId = id;
		Globals.getOuStre().write(Globals.SELECT_SAVERB);
		UtilFuncs.outputStreamSendInt64(Globals.getOuStre(), id);
		JStrTextAreaWithSynopsis textc = new JStrTextAreaWithSynopsis(this,SingleInstanceSense.recvSAVerb(id).gets(0));
		components.add(textc);
		//undone display all related single arg verbs here
		components.add(new ButtonWithFixedText(this,"SAVE"){
			@Override
			public void spacePressed() {
				if(textc.javaStr.isEmpty())
					return;
				try {
					if(SingleInstanceSense.updateSAVerb(snId, textc.javaStr)==0)
						showMsgBox("Updated successfully!");
					else
						showMsgBox("Update failed at server side");
				} catch (IOException e) {
					Globals.getLogger().log(Level.SEVERE, e.getMessage(), e);
					showMsgBox("Exception thrown when trying to update verb");
				}
				this.cg.context.redrawNeeded = true;
			}
		});
		components.add(new ButtonWithFixedText(this, "Open in lineage page"){
			@Override
			public void spacePressed() {
				try {
					this.cg.context.editor.addTabNSwitchToIt(new SingleArgVerbLineageEdit(this.cg.context,snId));
				} catch (IOException e) {
					Globals.getLogger().log(Level.SEVERE, e.getMessage(), e);
					showMsgBox("Exception thrown when trying to open SingleArgVerbLineageEdit page");
					this.cg.context.redrawNeeded = true;
				}
			}
		});
		components.add(new ButtonWithFixedText(this, "Select/Insert mutex groups containing this verb"){
			@Override
			public void spacePressed() {
				ComponentGroup newcg = new ComponentGroup(this.cg);
				newcg.components.add(new ButtonWithFixedText(newcg, "MAKE NEW GROUP"){
					@Override
					public void spacePressed() {
						try {
							this.cg.context.editor.addTabNSwitchToIt(SingleArgVerbMutexGroupEdit.makeNewGroup(this.cg.context,snId));
						} catch (IOException e) {
							Globals.getLogger().log(Level.SEVERE, e.getMessage(), e);
							showMsgBox("Exception thrown when trying to open SingleArgVerbMutexGroupEdit page");
							this.cg.context.redrawNeeded = true;
						}
					}
				});
				try {
					Globals.getOuStre().write(Globals.SELECTMUTEXCONTAININGVERB);
					UtilFuncs.outputStreamSendInt64(Globals.getOuStre(), snId);
					long lgid = 0;
					StringBuilder sb=null;
					while(true){
						long lbuf = UtilFuncs.inputStreamRecvInt64(Globals.getInStre());
						if(lbuf==0) break;
						if(lbuf!=lgid){
							if(lgid!=0)newcg.components.add(makeButtonOfMutexGroup(newcg, lgid, sb));
							lgid=lbuf;
							sb=new StringBuilder();
						}
						sb.append('`').append(UtilFuncs.getFirstLine(UtilFuncs.inputStreamRecvUtf8String(Globals.getInStre())));
					}
					if(lgid!=0) newcg.components.add(makeButtonOfMutexGroup(newcg, lgid, sb));
				} catch (IOException e) {
					Globals.getLogger().log(Level.SEVERE, e.getMessage(), e);
					showMsgBox("Exception thrown when selecting");
				}
				this.cg.modalD = newcg;
				this.cg.context.redrawNeeded = true;
			}
		});
		components.add(new ButtonWithFixedText(this, "DELETE this verb"){
			@Override
			public void spacePressed() {
				this.cg.modalD = new ComponentGroupCanBeClosedWithEsc(this.cg);
				this.cg.modalD.components.add(new ButtonWithFixedText(this.cg.modalD, "Confirm the deletion"){
					@Override
					public void spacePressed() {
						try {
							if(SingleInstanceSense.deleteSAVerb(snId)!=0){
								showMsgBox("Failed to complete deletion");
								this.cg.context.redrawNeeded = true;
								return;
							}
						} catch (IOException e) {
							Globals.getLogger().log(Level.SEVERE, e.getMessage(), e);
							showMsgBox("Exception thrown when trying to delete verb");
							this.cg.context.redrawNeeded = true;
							return;
						}
						this.cg.context.editor.tabs.removeLast();
						this.cg.context.editor.currentTab = this.cg.context.editor.tabs.getLast();
						this.cg.context.redrawNeeded = true;
					}
				});
				this.cg.context.redrawNeeded = true;
			}
		});
	}
	
	ButtonWithStriVal makeButtonOfMutexGroup(ComponentGroup newcg, final long teml, StringBuilder sb){
		return new ButtonWithStriVal(newcg, sb.substring(1)){
			@Override
			public long getl() {
				return teml;
			}
			@Override
			public void spacePressed() {
				try {
					this.cg.context.editor.addTabNSwitchToIt(new SingleArgVerbMutexGroupEdit(this.cg.context,this.getl()));
				} catch (IOException e) {
					Globals.getLogger().log(Level.SEVERE, e.getMessage(), e);
					showMsgBox("Exception thrown when trying to open SingleArgVerbMutexGroupEdit page");
					this.cg.context.redrawNeeded = true;
				}
			}
		};
	}

	@Override
	public String toString() {
		return "Verb Edit";
	}
}
