package griddoor.iiswingui;

public class QueToAnsWithStr extends QueToAns {

	public QueToAnsWithStr(CharSequence allText) {
		super(allText);
	}

	public String strans;

	@Override
	public void GetStrToFillJta() {
		IISUI.textInput.setText(strans);
	}

	@Override
	public void FuncStrTraverseFromJta(String str) {
		strans = str;
		RefrSelAs2ndLast();
		IISUI.tc.DrawTextAndSelection();
	}

	@Override
	public void RefrSelAs2ndLast() {
		selAs2ndLast.allText = "STRING ANS:\n" + strans;
	}

}
