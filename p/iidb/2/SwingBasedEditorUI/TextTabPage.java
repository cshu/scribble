package griddoor.SwingBasedEditorUI;

public class TextTabPage extends ComponentGroup{

	public int prependOffset;
	
	public TextTabPage(UIContext context) {
		super(context, null);
	}
	
	public void funcAfterFocus() {
	}
	
	@Override
	public void escPressed(){
		if(modalD==null){
			ComponentGroupCanBeClosedWithEsc modalDToUse = new ComponentGroupCanBeClosedWithEsc(this);
			modalDToUse.components.add(new ButtonWithFixedText(modalDToUse,"CLOSE THE TAB"){
				@Override
				public void spacePressed() {
					this.cg.context.editor.tabs.removeLast();
					this.cg.context.editor.currentTab = this.cg.context.editor.tabs.getLast();
					this.cg.context.redrawNeeded = true;
				}
			});
			modalD = modalDToUse;
			this.context.redrawNeeded = true;
		}else
			modalD.escPressed();
	}
	
}
