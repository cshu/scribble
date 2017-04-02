package griddoor.iiswingui;


public class ClauNSegSlot extends QueToAnsWithOpt {

	public ClauNSegSlot() {
		super(" [NSeg] ");
		opts = IISUI.tcCla.nSegs;
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
		refreshedSegsVerno = IISUI.tcCla.nSegsVerno;
	}
	
	@Override
	public void FuncFocusGained() {
		if(refreshedSegsVerno!=IISUI.tcCla.nSegsVerno)
			RefrSelAs2ndLast();
	}

}
