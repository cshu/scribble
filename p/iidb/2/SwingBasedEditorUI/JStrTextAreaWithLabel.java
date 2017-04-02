package griddoor.SwingBasedEditorUI;

public class JStrTextAreaWithLabel extends JavaStringTextArea {
	String label;
	public JStrTextAreaWithLabel(ComponentGroup cg, String label) {
		super(cg);
		this.label = label;
	}

	@Override
	public void fillByPrepending() {
		fillEditorWithLines(tyname+label+"\n");
	}
}
