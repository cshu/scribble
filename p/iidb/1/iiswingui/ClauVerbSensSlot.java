package griddoor.iiswingui;

import griddoor.iiswingui.IISUI.BI;

public class ClauVerbSensSlot extends ClauSensSlot {

	public ClauVerbSensSlot() {
		super();
	}

	@Override
	public void RefrSenseOptions() {
		int ind = 0;
		setmemb: {
			for (BI bi : IISUI.synonym)
				if (bi.a.contains(IISUI.textInput.getText()) && Utils.IsVerb(bi.b)) {
					optsAlias.set(ind++, new OptWithTypeAndBI(bi, "-unknown-", false));
					if (ind == 9)
						break setmemb;
				}
			for (BI bi : IISUI.inverseverb)
				if (bi.a.contains(IISUI.textInput.getText())) {
					optsAlias.set(ind++, new OptWithTypeAndBI(bi, "-unknown-", true));
					if (ind == 9)
						break setmemb;
				}
			for (; ind < 9; ind++)
				opts.set(ind, null);
		}
		optans = -1;
	}

	@Override
	public void FuncAfterFuncSpacePress() {
		FuncAfterFuncSpacePressForV();
	}
}
