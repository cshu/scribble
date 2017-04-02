package griddoor.SwingBasedEditorUI;

import griddoor.util.UtilFuncs;

public class JStrTextAreaWithSynopsis extends JavaStringTextArea {

	public JStrTextAreaWithSynopsis(ComponentGroup cg, String javaStr) {
		super(cg, javaStr);
	}

	public JStrTextAreaWithSynopsis(ComponentGroup cg) {
		super(cg);
	}

	@Override
	public void fillByPrepending() {
		fillEditorWithLines(tyname+UtilFuncs.getFirstLine(javaStr)+"\n");
	}
	@Override
	public void bindStrFunc() {
		super.bindStrFunc();
		this.cg.context.redrawNeeded = true;
	}
}
