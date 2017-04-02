package griddoor.iiSwBasedSpInterfaces;

import java.io.IOException;
import java.util.ArrayList;
import java.util.logging.Level;

import griddoor.IIPropSets.Formatters;
import griddoor.IIPropSets.ShortNote;
import griddoor.IIPropSets.SingleInstanceSense;
import griddoor.SwingBasedEditorUI.Button;
import griddoor.SwingBasedEditorUI.ButtonWithFixedText;
import griddoor.SwingBasedEditorUI.ButtonWithVal;
import griddoor.SwingBasedEditorUI.ComponentGroup;
import griddoor.SwingBasedEditorUI.EditorInterfaceJFrame;
import griddoor.SwingBasedEditorUI.JavaStringTextArea;
import griddoor.SwingBasedEditorUI.ModalGroup;
import griddoor.SwingBasedEditorUI.NewTabPageLink;
import griddoor.SwingBasedEditorUI.TextComponent;
import griddoor.SwingBasedEditorUI.TextTabPage;
import griddoor.SwingBasedEditorUI.UIContext;
import griddoor.util.SqliteUtil;
import griddoor.util.UtilFuncs;

public class ShortNoteTabPage extends TextTabPage {

	public ShortNoteTabPage(UIContext context) {
		super(context);
		
		components.add(new JavaStringTextArea(this,""){
			@Override
			public void fillByPrepending() {
				fillEditorWithLines(tyname+"Search short note related verbs\n");
			}
			
			@Override
			public void spacePressed(){
				//undone if bind value is empty, then list top level tags of short notes
				//undone you will be able to find CALENDAR notes here, these notes are associated with tmBytea data!
				this.cg.context.redrawNeeded = true;
			}
			//note cloneCom is not needed
		});
		components.add(new JavaStringTextArea(this,""){
			@Override
			public void fillByPrepending() {
				fillEditorWithLines(tyname+"Search all short notes' text\n");
			}
			
			@Override
			public void spacePressed(){
				if(this.javaStr.isEmpty())
					return;
				ArrayList<SingleInstanceSense> sisarr = new ArrayList<>();
				try {
					Globals.getOuStre().write(Globals.SEARCH_SHORTNOTETEXT);
					UtilFuncs.outputStreamSendStrCriterionNEscChar(Globals.getOuStre(), javaStr);
					while(true){
						SingleInstanceSense sis = SingleInstanceSense.recvShortNoteWithSynopsis();
						if(sis==null)
							break;
						sisarr.add(sis);
					}
				} catch (IOException e) {
					Globals.getLogger().log(Level.SEVERE, e.getMessage(), e);
					showMsgBox("Exception thrown when trying to search");
					this.cg.context.redrawNeeded = true;
					return;
				}
				if(sisarr.size()==0){
					showMsgBox("No short note found");
					this.cg.context.redrawNeeded = true;
					return;
				}
				this.cg.modalD = new ComponentGroup(this.cg);
				for(SingleInstanceSense sis : sisarr){
					this.cg.modalD.components.add(new ButtonWithVal<SingleInstanceSense>(this.cg.modalD, sis){
						@Override
						public void spacePressed() {
							try {
								this.cg.context.editor.addTabNSwitchToIt(new ShortNoteEdit(this.cg.context,val.id));
							} catch (IOException e) {
								Globals.getLogger().log(Level.SEVERE, e.getMessage(), e);
								showMsgBox("Exception thrown when trying to open a short note");
								this.cg.context.redrawNeeded = true;
							}
						}
						@Override
						public void fillByPrepending() {
							fillEditorWithLines(Formatters.formatAsShortNoteSynopsis(this.val).append('\n').toString());
						}
					});
				}
				this.cg.context.redrawNeeded = true;
			}
		});
		components.add(new ButtonWithFixedText(this, "Write a note"){
			@Override
			public void spacePressed(){
				this.cg.context.editor.addTabNSwitchToIt(new ShortNoteEdit(this.cg.context));
//				JavaStringTextArea noteTextArea = new JavaStringTextArea(this.tabPage, ""){
//					@Override
//					public void FillByAppending() {
//						tabPage.fillEditorWithLines(tyname+"Text of short note\n");
//					}
//				};
//				ShortNote sn = new ShortNote();
//				components.add(noteTextArea);
//				components.add(new ModalGroup(this.tabPage){
//					//undone add verbs together with this short note
//				});
//				components.add(new Button(this.tabPage){
//					@Override
//					public void FillByAppending() {
//						tabPage.fillEditorWithLines(tyname+"Save short note\n");
//					}
//					@Override
//					public void spacePressed() {
//						if(noteTextArea.javaStr.isEmpty())
//							return;
//						sn.javaStr = noteTextArea.javaStr;
//						//undone send all props to server, after it's done, close the modal dialog
//						//undone this should easily succeed, unless the same blob as short note happens to exist already, in that case saving prop fails
//					}
//				});
				//undone
//				tabPage.editorInter.updateEditorText();
			}
		});
		
	}
	public static NewTabPageLink instantiateNewTabPageLink(ComponentGroup cgOfLink){
		return new NewTabPageLink(cgOfLink){
			@Override
			public NewTabPageLink cloneCom(ComponentGroup newcg) {
				return instantiateNewTabPageLink(newcg);
			}
			@Override
			public void fillByPrepending() {
				fillEditorWithLines(tyname+"Short note page in new tab\n");
			}
			@Override
			public void spacePressed() {
				this.cg.context.editor.addTabNSwitchToIt(new ShortNoteTabPage(this.cg.context));
			}
		};
	}
	@Override
	public String toString() {
		return "Short Notes";
	}
}
