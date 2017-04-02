package griddoor.iiswingui;

public class QueToAns extends Selectable {

	public QueToAns(CharSequence allText) {
		super(allText);
	}

	public Selectable selAs2ndLast = new Selectable();
	
	@Override
	public Selectable GetSelFor2ndLastSel() {
		return selAs2ndLast;
	}

	@Override
	public void FuncXReleasedInTCSens() {
		IISUI.tc.croppedSel1 = this;
		IISUI.tc.sels.set(IISUI.tc.ind, Utils.MacrRefrSelAs2ndLast(new SensSlot()));
		IISUI.tc.sels.add(IISUI.tc.ind+1,Utils.MacrRefrSelAs2ndLast(new SensCategory()));

		IISUI.tc.Set2ndLastSel();
		IISUI.tc.CalcSelectionPos();
		IISUI.tc.DrawTextAndSelection();
	}
	
	@Override
	public void FuncPReleasedInTCSens() {
		if(IISUI.tc.croppedSel1==null)
			return;
		if(IISUI.tc.croppedSel1 instanceof SensSlot)
			IISUI.tc.sels.add(IISUI.tc.ind+1, IISUI.tc.croppedSel2);
		IISUI.tc.sels.set(IISUI.tc.ind, IISUI.tc.croppedSel1);
		IISUI.tc.croppedSel1 = null;

		IISUI.tc.Set2ndLastSel();
		IISUI.tc.CalcSelectionPos();
		IISUI.tc.DrawTextAndSelection();
	}
}
