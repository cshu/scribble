package griddoor.iiswingui;

import java.awt.event.KeyEvent;

public class Selectable implements CharSequence {
	public CharSequence allText;

	public Selectable() {
	}

	public Selectable(CharSequence allText) {
		this.allText = allText;
	}

	public void RefrSelAs2ndLast() {

	}

	public Selectable GetSelFor2ndLastSel() {
		return IISUI.newlineSel;
	}

	public void FuncKeyPressed(KeyEvent e) {
	}

	public void GetStrToFillJta() {
		IISUI.textInput.setText(IISUI.textInputCopies.size() > 0 ? IISUI.textInputCopies.get(IISUI.textInputCopies.size() - 1) : "");
	}

	public void FuncStrTraverseFromJta(String str) {
	}

	@Override
	public int length() {
		return allText.length();
	}

	@Override
	public char charAt(int index) {
		return allText.charAt(index);
	}

	@Override
	public CharSequence subSequence(int start, int end) {
		return allText.subSequence(start, end);
	}

	@Override
	public String toString() {
		return allText.toString();
	}

	public void FuncXReleasedInTCSens() {
	}

	public void FuncPReleasedInTCSens() {
	}

	public void FuncInsOrRemUpdate() {

	}

	public void FuncFocusGained() {
	}

}
