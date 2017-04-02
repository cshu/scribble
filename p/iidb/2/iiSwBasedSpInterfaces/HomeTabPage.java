package griddoor.iiSwBasedSpInterfaces;

import java.io.IOException;
import java.util.logging.Level;

import griddoor.SwingBasedEditorUI.ButtonWithFixedText;
import griddoor.SwingBasedEditorUI.ComponentGroup;
import griddoor.SwingBasedEditorUI.TextTabPage;
import griddoor.SwingBasedEditorUI.UIContext;

public class HomeTabPage extends TextTabPage {

	public HomeTabPage(UIContext context) {
		super(context);
		components.add(ShortNoteTabPage.instantiateNewTabPageLink(this));
		components.add(ShortNoteAdvSearch.instantiateNewTabPageLink(this));
		components.add(UrlBookmarkTabPage.instantiateNewTabPageLink(this));
		components.add(UrlBookmarkAdvSearch.instantiateNewTabPageLink(this));
		components.add(NaturalLangTabPage.instantiateNewTabPageLink(this));
		//components.add(AuthInfoTabPage.instantiateNewTabPageLink(this));
		components.add(FileSystemTabPage.instantiateNewTabPageLink(this));
		components.add(MalAnimeTabPage.instantiateNewTabPageLink(this));
		components.add(SingleArgVerbTabPage.instantiateNewTabPageLink(this));
		components.add(instantiateCloseConnBtn(this));
		components.add(instantiateExitBtn(this));
	}
	@Override
	public void escPressed() {
		//todo anything needed here??
	}

	public static ButtonWithFixedText instantiateCloseConnBtn(ComponentGroup cgOfNewInst){
		return new ButtonWithFixedText(cgOfNewInst, "Close connection to DB"){
			@Override
			public void spacePressed(){
				//undone ask for confirmation
				try {
					Globals.closSocketIfNotNull();
				} catch (IOException e) {
					Globals.getLogger().log(Level.SEVERE, e.getMessage(), e);
					showMsgBox("Exception thrown when trying to close socket");
					this.cg.context.redrawNeeded = true;
					return;
				}
				this.cg.context.redrawNeeded = true;
			}
			@Override
			public ButtonWithFixedText cloneCom(ComponentGroup newcg) {
				return instantiateCloseConnBtn(newcg);
			}
		};
	}
	
	public static ButtonWithFixedText instantiateExitBtn(ComponentGroup cgOfNewInst){
		return new ButtonWithFixedText(cgOfNewInst, "Exit"){
			@Override
			public void spacePressed(){
				if(Globals.clientSocket==null)
					System.exit(0);
			}
			@Override
			public ButtonWithFixedText cloneCom(ComponentGroup newcg) {
				return instantiateExitBtn(newcg);
			}
		};
	}
	@Override
	public String toString() {
		return "HOME";
	}
}
