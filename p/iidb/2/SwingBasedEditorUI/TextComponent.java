package griddoor.SwingBasedEditorUI;

import griddoor.util.PrimaryStruct;

public abstract class TextComponent extends PrimaryStruct{

	//public TextTabPage tabPage;
	public final ComponentGroup cg;
	public TextComponent(ComponentGroup cg) {
		this.cg = cg;
	}
	abstract public boolean isSelectable();
	public String getTooltip(){
		return "";
	}
	abstract public TextComponent cloneCom(ComponentGroup newcg);

	public void xPressed(){
	}
	public void spacePressed() {
	}
	public void bindStrFunc(){
	}
	public String strForInsMode(){
		return null;
	}
	public String getJavaStringFromThisAsTextArea(){
		return null;
	}
	
	abstract public void fillByPrepending();
	
	public void fillEditorWithLines(String... lines){
		for(String line : lines){
			if(cg.context.editor.currentTab.prependOffset<=cg.context.prependLineCount && cg.context.prependLineCount<cg.context.editor.currentTab.prependOffset+Globals.maxNumOfLines){
				Globals.makeLineAbbr(cg.context.sp, line);
				cg.context.sp.prepend(cg.context.charToUseForLine);
			}
			cg.context.prependLineCount++;
		}
	}
	public void showMsgBox(String line){
		ComponentGroup modalDToUse = new ComponentGroup(cg); 
		modalDToUse.components.add(new CloseButton(modalDToUse,line));
		cg.modalD = modalDToUse;
	}
}
