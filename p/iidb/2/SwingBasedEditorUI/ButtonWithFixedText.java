package griddoor.SwingBasedEditorUI;

public class ButtonWithFixedText extends Button {

	
	public String fixedText;
	public ButtonWithFixedText(ComponentGroup cg, String fixedText) {
		super(cg);
		this.fixedText=fixedText;
	}
	
	@Override
	public void fillByPrepending() {
		fillEditorWithLines(tyname+fixedText+"\n");
	}
	
	@Override
	public TextComponent cloneCom(ComponentGroup newcg) {
		return new ButtonWithFixedText(newcg, fixedText);
	}
}
