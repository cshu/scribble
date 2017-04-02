package griddoor.iiswingui;

import java.util.Arrays;

public class SensCategory extends QueToAnsWithOpt {

	public SensCategory() {
		super(" [Category]>");
		opts = Arrays.<CharSequence>asList( "SYN/INV","noun", "verb", "concrete noun", "abstract noun", "invertible verb", "noninvertible verb", "byte noun", "digit noun" );
	}

	@Override
	public void FuncXReleasedInTCSens() {
		IISUI.tc.croppedSel1 = IISUI.tc.sels.get(IISUI.tc.ind-1);
		IISUI.tc.croppedSel2 = this;
		IISUI.tc.sels.set(IISUI.tc.ind-1,Utils.MacrRefrSelAs2ndLast(new SensSlot()));
		IISUI.tc.sels.set(IISUI.tc.ind,Utils.MacrRefrSelAs2ndLast(new SensCategory()));
		
		IISUI.tc.Set2ndLastSel();
		IISUI.tc.CalcSelectionPos();
		IISUI.tc.DrawTextAndSelection();
	}
	
	@Override
	public void FuncPReleasedInTCSens() {
		if(IISUI.tc.croppedSel1==null)
			return;
		if(IISUI.tc.croppedSel1 instanceof SensSlot)
			IISUI.tc.sels.set(IISUI.tc.ind, IISUI.tc.croppedSel2);
		else
			IISUI.tc.sels.remove(IISUI.tc.ind);
		IISUI.tc.ind--;
		IISUI.tc.sels.set(IISUI.tc.ind, IISUI.tc.croppedSel1);
		IISUI.tc.croppedSel1 = null;
		
		IISUI.tc.Set2ndLastSel();
		IISUI.tc.CalcSelectionPos();
		IISUI.tc.DrawTextAndSelection();
	}
}
