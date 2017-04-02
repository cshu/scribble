package griddoor.iiswingui;

public class ClauVSegForAdveSlot extends ClauVSegSlot {

	public ClauVSegForAdveSlot() {
	}

	@Override
	public void FuncSpacePress() {
		if (optans >= 0 && opts.get(optans) != null && opts.get(optans).toString().startsWith("adverbialcl")) {
			allText = opts.get(optans);

			IISUI.tc.CalcSelectionPos();
			IISUI.tc.DrawTextAndSelection();
		}
	}
}
