package griddoor.iiSwBasedSpInterfaces;

import griddoor.SwingBasedEditorUI.ComponentGroup;
import griddoor.SwingBasedEditorUI.NewTabPageLink;
import griddoor.SwingBasedEditorUI.TextTabPage;
import griddoor.SwingBasedEditorUI.UIContext;

//including electronic accounts etc
public class AuthInfoTabPage extends TextTabPage {

	public AuthInfoTabPage(UIContext context) {
		super(context);
	}
	public static NewTabPageLink instantiateNewTabPageLink(ComponentGroup cgOfLink){
		return new NewTabPageLink(cgOfLink){
			@Override
			public NewTabPageLink cloneCom(ComponentGroup newcg) {
				return instantiateNewTabPageLink(newcg);
			}

			@Override
			public void fillByPrepending() {
				fillEditorWithLines(tyname+"Auth info page in new tab\n");
			}
			@Override
			public void spacePressed() {
				this.cg.context.editor.addTabNSwitchToIt(new AuthInfoTabPage(this.cg.context));
			}
		};
	}
}
