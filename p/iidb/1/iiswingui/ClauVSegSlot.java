package griddoor.iiswingui;


public class ClauVSegSlot extends QueToAnsWithOpt {

	public ClauVSegSlot() {
		super(" [VSeg] ");
		opts = IISUI.tcCla.vSegs;
	}
	
	@Override
	public void FuncSpacePress() {
		if (optans >= 0 && opts.get(optans) != null) {
			allText = opts.get(optans);

			IISUI.tc.CalcSelectionPos();
			IISUI.tc.DrawTextAndSelection();
		}
	}
	
	public int refreshedSegsVerno;
	
	@Override
	public void RefrSelAs2ndLast() {
		super.RefrSelAs2ndLast();
		refreshedSegsVerno = IISUI.tcCla.vSegsVerno;
	}
	
	@Override
	public void FuncFocusGained() {
		if(refreshedSegsVerno!=IISUI.tcCla.vSegsVerno)
			RefrSelAs2ndLast();
	}
}
