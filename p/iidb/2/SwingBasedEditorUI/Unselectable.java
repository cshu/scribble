package griddoor.SwingBasedEditorUI;

//be careful unselectable is not considered for a lot of features, you should not use this class unless you ensure safety
public abstract class Unselectable extends TextComponent {


	public Unselectable(ComponentGroup cg) {
		super(cg);
	}
	
	@Override
	public final boolean isSelectable() {
		return false;
	}
}
