package griddoor.SwingBasedEditorUI;

public abstract class Selectable extends TextComponent {
	
	public static final String tyname = "`";

	public Selectable(ComponentGroup cg) {
		super(cg);
	}
	
	@Override
	public final boolean isSelectable() {
		return true;
	}

	
}
