package griddoor.SwingBasedEditorUI;

public class JStrTextAreaWithLabelNSynopsis extends JavaStringTextArea {

	String labelWithG;
	public JStrTextAreaWithLabelNSynopsis(ComponentGroup cg, String labelWithG) {
		super(cg);
		this.labelWithG = labelWithG;
	}
	public JStrTextAreaWithLabelNSynopsis(ComponentGroup cg, String labelWithG, String jstr) {
		this(cg, labelWithG);
		javaStr=jstr;
	}

	@Override
	public void fillByPrepending() {
		fillEditorWithLines(tyname+labelWithG+javaStr+"\n");
	}
	
	@Override
	public void bindStrFunc() {
		super.bindStrFunc();
		this.cg.context.redrawNeeded = true;
	}
}
