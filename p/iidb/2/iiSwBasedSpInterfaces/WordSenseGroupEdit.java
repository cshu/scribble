package griddoor.iiSwBasedSpInterfaces;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.net.UnknownHostException;
import java.util.HashSet;
import java.util.logging.Level;

import griddoor.SwingBasedEditorUI.ButtonWithFixedText;
import griddoor.SwingBasedEditorUI.ComponentGroupCanBeClosedWithEsc;
import griddoor.SwingBasedEditorUI.JStrTextAreaWithLabel;
import griddoor.SwingBasedEditorUI.JStrTextAreaWithSynopsis;
import griddoor.SwingBasedEditorUI.TextTabPage;
import griddoor.SwingBasedEditorUI.UIContext;
import griddoor.util.UtilFuncs;

public class WordSenseGroupEdit extends TextTabPage {

	public long snId;
	public WordSenseGroupEdit(UIContext context){
		super(context);
		addRefrBtn();
		addBtns();
	}
	public void addRefrBtn(){
		components.add(new ButtonWithFixedText(this, "Refresh and discard all changes"){
			@Override
			public void spacePressed() {
				try {
					this.cg.context.editor.currentTab = snId==0? new WordSenseGroupEdit(context) : new WordSenseGroupEdit(context,snId);
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
	}
	public void addBtns(){
		components.add(new JStrTextAreaWithLabel(this, "Bind to add essential text (Or press space to edit props)"){
			@Override
			public void bindStrFunc() {
				this.cg.components.add(this.cg.cursorIndex, makeJStrTextAreaForText(cg.context.editor.textInput.getText()));
				this.cg.context.redrawNeeded = true;
			}
			@Override
			public void spacePressed() {
				if(snId==0)
					showMsgBox("This is a new word sense group. You cannot edit props.");
				else
					try {
						this.cg.context.editor.addTabNSwitchToIt(new SingleArgPropsEdit(this.cg.context,snId,"[Word Sense Group] "+UtilFuncs.getFirstLine(this.javaStr)));
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
				if(this.cg.cursorIndex==2){
					showMsgBox("Cannot save empty word sense group.");
					this.cg.context.redrawNeeded = true;
					return;
				}
				HashSet<String> allwords = new HashSet<>();
				for(int i=1;i<this.cg.cursorIndex-1;i++){
					String word = this.cg.components.get(i).getJavaStringFromThisAsTextArea();
					if(word.isEmpty()){
						showMsgBox("Cannot save empty entry.");
						this.cg.context.redrawNeeded = true;
						return;
					}
					if(!allwords.add(word)){
						showMsgBox("Cannot save duplicate entry.");
						this.cg.context.redrawNeeded = true;
						return;
					}
				}
				try {
					Globals.getOuStre().write(Globals.UPSERTORDELETE_WORDSENSEGROUP);
					UtilFuncs.outputStreamSendInt64(Globals.getOuStre(), snId);
					for(String word : allwords){
						Globals.getOuStre().write(1);
						UtilFuncs.outputStreamSendLen24AndStrUtf8(Globals.getOuStre(), word);
					}
					Globals.getOuStre().write(0);
					if(Globals.getInStre().read()==0){
						snId = UtilFuncs.inputStreamRecvInt64(Globals.getInStre());
						showMsgBox("Saved successfully");
					}else
						showMsgBox("Failed to complete update/insertion");
				} catch (IOException e) {
					Globals.getLogger().log(Level.SEVERE, e.getMessage(), e);
					showMsgBox("Exception thrown when trying to upsert");
				}
				this.cg.context.redrawNeeded = true;
			}
		});
		components.add(new ButtonWithFixedText(this,"DELETE"){
			@Override
			public void spacePressed() {
				if(snId==0){
					showMsgBox("This is a new word sense group. You cannot delete it.");
				}else{
					this.cg.modalD = new ComponentGroupCanBeClosedWithEsc(this.cg);
					this.cg.modalD.components.add(new ButtonWithFixedText(this.cg.modalD, "Confirm the deletion"){
						@Override
						public void spacePressed() {
							try {
								Globals.getOuStre().write(Globals.UPSERTORDELETE_WORDSENSEGROUP);
								UtilFuncs.outputStreamSendInt64(Globals.getOuStre(), snId);
								Globals.getOuStre().write(0);
								if(Globals.getInStre().read()!=0){
									showMsgBox("Failed to complete deletion");
									this.cg.context.redrawNeeded = true;
									return;
								}
								UtilFuncs.inputStreamRecvInt64(Globals.getInStre());//note discard the received id
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
	public WordSenseGroupEdit(UIContext context, long id) throws UnknownHostException, IOException {
		super(context);
		this.snId = id;
		Globals.getOuStre().write(Globals.SELECT_ESSENTIALTEXT);
		UtilFuncs.outputStreamSendInt64(Globals.getOuStre(), snId);
		addRefrBtn();
		while(Globals.getInStre().read()!=0)
			components.add(makeJStrTextAreaForText(UtilFuncs.inputStreamRecvUtf8String(Globals.getInStre())));
		addBtns();
	}
	
	JStrTextAreaWithSynopsis makeJStrTextAreaForText(String t){
		return new JStrTextAreaWithSynopsis(this, t){
			@Override
			public void xPressed() {
				this.cg.components.remove(this.cg.cursorIndex);
				this.cg.context.redrawNeeded = true;
			}
		};
	}

}
