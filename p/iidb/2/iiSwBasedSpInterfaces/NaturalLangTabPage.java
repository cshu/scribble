package griddoor.iiSwBasedSpInterfaces;

import java.io.IOException;
import java.util.logging.Level;

import griddoor.SwingBasedEditorUI.ButtonWithFixedText;
import griddoor.SwingBasedEditorUI.ButtonWithVal;
import griddoor.SwingBasedEditorUI.ComponentGroup;
import griddoor.SwingBasedEditorUI.JavaStringTextArea;
import griddoor.SwingBasedEditorUI.NewTabPageLink;
import griddoor.SwingBasedEditorUI.TextTabPage;
import griddoor.SwingBasedEditorUI.UIContext;
import griddoor.util.PrimaryStruct;
import griddoor.util.UtilFuncs;

public class NaturalLangTabPage extends TextTabPage {

	public NaturalLangTabPage(UIContext context) {
		super(context);
		components.add(new JavaStringTextArea(this, ""){
			@Override
			public void fillByPrepending() {
				fillEditorWithLines(tyname+"Search for words\n");
			}
			@Override
			public void spacePressed() {
				if(javaStr.isEmpty())
					return;
				ComponentGroup newcg = new ComponentGroup(this.cg);
				try {
					Globals.getOuStre().write(Globals.SEARCH_WORDSENSEGROUP);
					UtilFuncs.outputStreamSendStrCriterionNEscChar(Globals.getOuStre(), javaStr);
					while(true){
						long lbuf = UtilFuncs.inputStreamRecvInt64(Globals.getInStre());
						if(lbuf==0)
							break;
						PrimaryStruct lns = PrimaryStruct.make(lbuf,UtilFuncs.inputStreamRecvUtf8String(Globals.getInStre()));
						StringBuilder str = new StringBuilder(lns.gets());
						while(Globals.getInStre().read()!=0)
							str.append('`').append(UtilFuncs.inputStreamRecvUtf8String(Globals.getInStre()));
						String buttonstr = str.append('\n').toString();
						newcg.components.add(new ButtonWithVal<PrimaryStruct>(newcg,lns){
							@Override
							public void fillByPrepending() {
								fillEditorWithLines(buttonstr);
							};
							@Override
							public void spacePressed() {
								try {
									this.cg.context.editor.addTabNSwitchToIt(new WordSenseGroupEdit(this.cg.context,this.val.getl()));
								} catch (IOException e) {
									Globals.getLogger().log(Level.SEVERE, e.getMessage(), e);
									showMsgBox("Exception thrown when trying to open edit tab");
									this.cg.context.redrawNeeded = true;
								}
							};
						});
					}
				} catch (IOException e) {
					Globals.getLogger().log(Level.SEVERE, e.getMessage(), e);
					showMsgBox("Exception thrown when trying to search");
					this.cg.context.redrawNeeded = true;
					return;
				}
				if(newcg.components.isEmpty())
					showMsgBox("Nothing found");
				else
					this.cg.modalD = newcg;
				this.cg.context.redrawNeeded = true;
			}
		});
		components.add(new ButtonWithFixedText(this, "Make a word sense group"){
			@Override
			public void spacePressed(){
				this.cg.context.editor.addTabNSwitchToIt(new WordSenseGroupEdit(this.cg.context));
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
				fillEditorWithLines(tyname+"Natural language page in new tab\n");
			}
			@Override
			public void spacePressed() {
				this.cg.context.editor.addTabNSwitchToIt(new NaturalLangTabPage(this.cg.context));
			}
		};
	}

	@Override
	public String toString() {
		return "Natural Language";
	}
}
