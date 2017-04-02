package griddoor.SwingBasedEditorUI;

import griddoor.util.UtilFuncs;

public class JavaStringTextArea extends Selectable {
	
	public static final String tyname = "TextArea`";

	public JavaStringTextArea(ComponentGroup cg, String javaStr) {
		super(cg);
		this.javaStr = javaStr;
	}
	public JavaStringTextArea(ComponentGroup cg) {
		super(cg);
		this.javaStr = "";
	}

	public String javaStr;
	@Override
	public String getJavaStringFromThisAsTextArea() {
		return javaStr;
	}
	@Override
	public String getTooltip() {
		//undone check maxNumOfLinesOfTooltip and maxLengthOfTooltip
		return UtilFuncs.getFirstLine(javaStr);
	}
	@Override
	public void bindStrFunc() {
		javaStr = cg.context.editor.textInput.getText();
	}
	@Override
	public String strForInsMode() {
		return javaStr;
	}
	
	@Override
	public void fillByPrepending() {
		fillEditorWithLines(tyname+"\n");//note javaStr not appended here coz you don't know if it contains \n
	}
	@Override
	public TextComponent cloneCom(ComponentGroup newcg) {
		return new JavaStringTextArea(newcg, javaStr);
	}
}
