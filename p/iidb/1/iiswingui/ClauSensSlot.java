package griddoor.iiswingui;

import java.util.ArrayList;
import java.util.Collections;

import griddoor.iiswingui.IISUI.BI;

public abstract class ClauSensSlot extends QueToAnsWithOpt {


	public ArrayList<OptWithTypeAndBI> optsAlias = new ArrayList<OptWithTypeAndBI>(Collections.<OptWithTypeAndBI>nCopies(9, null));
	
	public BI syn;
	
	public ClauSensSlot() {
		super(" SENSE ");
		opts = optsAlias;
		RefrSenseOptions();
	}

	public abstract void RefrSenseOptions();
	
	@Override
	public void FuncSpacePress() {
		if (optans >= 0 && opts.get(optans) != null) {
			syn = optsAlias.get(optans).syn;
			allText = syn.a;
			FuncAfterFuncSpacePress();

			IISUI.tc.CalcSelectionPos();
			IISUI.tc.DrawTextAndSelection();
		}
	}
	
	public void FuncAfterFuncSpacePress() {
	}
	
	public void FuncAfterFuncSpacePressForN(){
		CharSequence segCs = IISUI.tc.sels.get(IISUI.tc.ind - 1).allText;
		int i = IISUI.tcCla.nSegs.indexOf(segCs);
		if(i!=-1)
			return;
		i = IISUI.tcCla.nSegs.indexOf(null);
		if (i == -1)
			IISUI.tcCla.nSegs.add(segCs);
		else
			IISUI.tcCla.nSegs.set(i, segCs);
		IISUI.tcCla.nSegsVerno++;
	}
	
	public void FuncAfterFuncSpacePressForV(){
		CharSequence segCs = IISUI.tc.sels.get(IISUI.tc.ind - 1).allText;
		int i = IISUI.tcCla.vSegs.indexOf(segCs);
		if(i!=-1)
			return;
		i = IISUI.tcCla.vSegs.indexOf(null);
		if (i == -1)
			IISUI.tcCla.vSegs.add(segCs);
		else
			IISUI.tcCla.vSegs.set(i, segCs);
		IISUI.tcCla.vSegsVerno++;
	}

	@Override
	public void FuncInsOrRemUpdate() {
		RefrSenseOptions();
		RefrSelAs2ndLast();
		IISUI.tc.DrawTextAndSelection();
	}
}
