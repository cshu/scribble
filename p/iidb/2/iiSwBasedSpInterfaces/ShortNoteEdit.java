package griddoor.iiSwBasedSpInterfaces;

import java.io.IOException;
import java.net.UnknownHostException;
import java.util.logging.Level;

import griddoor.IIPropSets.SingleInstanceSense;
import griddoor.SwingBasedEditorUI.ButtonWithFixedText;
import griddoor.SwingBasedEditorUI.ButtonWithVal;
import griddoor.SwingBasedEditorUI.ComponentGroup;
import griddoor.SwingBasedEditorUI.ComponentGroupCanBeClosedWithEsc;
import griddoor.SwingBasedEditorUI.JStrTextAreaWithSynopsis;
import griddoor.SwingBasedEditorUI.JavaStringTextArea;
import griddoor.SwingBasedEditorUI.TextTabPage;
import griddoor.SwingBasedEditorUI.UIContext;
import griddoor.util.UtilFuncs;

public class ShortNoteEdit extends TextTabPage {

	public long snId;
	
	public ShortNoteEdit(UIContext context) {
		super(context);
		JavaStringTextArea snTextCom = new JStrTextAreaWithSynopsis(this);
		components.add(snTextCom);
		addComs(snTextCom);
	}

	public ShortNoteEdit(UIContext context, long id) throws UnknownHostException, IOException {
		super(context);
		snId = id;
		Globals.getOuStre().write(Globals.SELECT_SHORTNOTE);
		UtilFuncs.outputStreamSendInt64(Globals.getOuStre(), id);
		SingleInstanceSense sis = SingleInstanceSense.recvShortNoteInstance(id);
		JavaStringTextArea snTextCom = new JStrTextAreaWithSynopsis(this, sis.gets(Globals.SHORTNOTE_text)){
			@Override
			public void spacePressed() {
				//undone refresh!
			}
		};
		components.add(snTextCom);
		for(Long l : sis.lons){
			components.add(new ButtonWithVal<Long>(this,l){
				@Override
				public void fillByPrepending() {
					fillEditorWithLines(String.valueOf(SingleInstanceSense.allsenses.get(l))+'\n');
				}
			});
		}
		addComs(snTextCom);
	}
	
	public void addComs(JavaStringTextArea snTextCom){
		components.add(new ButtonWithFixedText(this, "Edit props"){
			@Override
			public void spacePressed() {
				if(snId==0)
					showMsgBox("This is a new note. You cannot proceed.");
				else
					try {
						this.cg.context.editor.addTabNSwitchToIt(new SingleArgPropsEdit(this.cg.context,snId,"[Short Note] "+UtilFuncs.getFirstLine(snTextCom.javaStr)));
					} catch (IOException e) {
						Globals.getLogger().log(Level.SEVERE, e.getMessage(), e);
						showMsgBox("Exception thrown when trying to load edit page");
					}
				this.cg.context.redrawNeeded = true;
			}
		});
		components.add(new ButtonWithFixedText(this, "Update (Insert) Database"){
			@Override
			public void spacePressed() {
				if(snTextCom.javaStr.isEmpty())
					return;//todo show a msg cannot be empty?
				this.cg.modalD = new ComponentGroupCanBeClosedWithEsc(this.cg);
				this.cg.modalD.components.add(new ButtonWithFixedText(this.cg.modalD, "Update (Insert) text only"){
					@Override
					public void spacePressed() {
						try {
							snId = SingleInstanceSense.upsertShortNoteTextOnly(snId,snTextCom.javaStr);
						} catch (IOException e) {
							Globals.getLogger().log(Level.SEVERE, e.getMessage(), e);
							showMsgBox("Exception thrown when trying to upsert shortnote");
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
		components.add(new ButtonWithFixedText(this, "DELETE this short note"){
			@Override
			public void spacePressed() {
				if(snId==0){
					showMsgBox("This is a new note. You cannot delete it.");
				}else{
					this.cg.modalD = new ComponentGroupCanBeClosedWithEsc(this.cg);
					this.cg.modalD.components.add(new ButtonWithFixedText(this.cg.modalD, "Confirm the deletion"){
						@Override
						public void spacePressed() {
							try {
								if(SingleInstanceSense.deleteShortNote(snId)!=0){
									showMsgBox("Failed to complete short note deletion");
									this.cg.context.redrawNeeded = true;
									return;
								}
							} catch (IOException e) {
								Globals.getLogger().log(Level.SEVERE, e.getMessage(), e);
								showMsgBox("Exception thrown when trying to delete short note");
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

	@Override
	public String toString() {
		return "Short Note Edit Page";
	}
}
