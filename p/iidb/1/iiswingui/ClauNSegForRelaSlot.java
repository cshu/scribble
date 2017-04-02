package griddoor.iiswingui;

public class ClauNSegForRelaSlot extends ClauNSegSlot {

	public ClauNSegForRelaSlot() {
	}

	@Override
	public void FuncSpacePress() {
		if (optans >= 0 && opts.get(optans) != null && opts.get(optans).toString().startsWith("relativecla")) {
			allText = opts.get(optans);
			
			IISUI.tc.CalcSelectionPos();
			IISUI.tc.DrawTextAndSelection();
		}
	}
}
