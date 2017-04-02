package griddoor.iiswingui;

import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;

import griddoor.iiswingui.IISUI.BI;
import griddoor.util.UtilFuncs;

public class SensSlot extends QueToAnsWithEitherOptOrStr {

	public ArrayList<OptWithTypeAndBI> optsAlias = new ArrayList<OptWithTypeAndBI>(Collections.<OptWithTypeAndBI>nCopies(9, null));

	public SensSlot() {// todo: add construnctor parameter for filtering things
		super(" <SENSE");
		opts = optsAlias;
		RefrSenseOptions();
	}

	public void RefrSenseOptions() {
		// todo: get part of speech, write a util func to do that
		// todo and use OptWithTypeAndBI to provide more info on screen
		int ind = 0;
		setmemb: {
			for (BI bi : IISUI.synonym)
				if (strans == null || bi.a.contains(strans)) {
					optsAlias.set(ind++, new OptWithTypeAndBI(bi, "-unknown-", false));
					if (ind == 9)
						break setmemb;
				}
			for (BI bi : IISUI.inverseverb)
				if (strans == null || bi.a.contains(strans)) {
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
	public void FuncInsOrRemUpdate() {
		strans = IISUI.textInput.getText();
		RefrSenseOptions();
		RefrSelAs2ndLast();
		IISUI.tc.DrawTextAndSelection();
	}

	@Override
	public void FuncXReleasedInTCSens() {
		IISUI.tc.croppedSel1 = this;
		IISUI.tc.croppedSel2 = IISUI.tc.sels.get(IISUI.tc.ind + 1);
		IISUI.tc.sels.set(IISUI.tc.ind, Utils.MacrRefrSelAs2ndLast(new SensSlot()));
		IISUI.tc.sels.set(IISUI.tc.ind + 1, Utils.MacrRefrSelAs2ndLast(new SensCategory()));

		IISUI.tc.Set2ndLastSel();
		IISUI.tc.CalcSelectionPos();
		IISUI.tc.DrawTextAndSelection();
	}

	@Override
	public void FuncPReleasedInTCSens() {
		if (IISUI.tc.croppedSel1 == null)
			return;
		if (IISUI.tc.croppedSel1 instanceof SensSlot)
			IISUI.tc.sels.set(IISUI.tc.ind + 1, IISUI.tc.croppedSel2);
		else
			IISUI.tc.sels.remove(IISUI.tc.ind + 1);
		IISUI.tc.sels.set(IISUI.tc.ind, IISUI.tc.croppedSel1);
		IISUI.tc.croppedSel1 = null;

		IISUI.tc.Set2ndLastSel();
		IISUI.tc.CalcSelectionPos();
		IISUI.tc.DrawTextAndSelection();
	}

	@Override
	public void FuncWPress() {
		if (strans == null)
			return;
		for (BI bi : IISUI.synonym)
			if (bi.a.equals(strans))
				return;
		for (BI bi : IISUI.inverseverb)
			if (bi.a.equals(strans))
				return;
		BI memb = IISUI.theJfr.new BI();
		byte[] buf = strans.toString().getBytes(StandardCharsets.UTF_8);

		try {
			IISUI.outpStreSo.write(1);
			IISUI.outpStreSo.write(17);
			IISUI.outpStreSo.write(1);
			IISUI.outpStreSo.write(buf.length % 256);
			IISUI.outpStreSo.write(buf.length / 256);
			IISUI.outpStreSo.write(buf);
			UtilFuncs.InputStreamReadBytesUntil(8, IISUI.inpuStreSo, memb.b, 0);

			// todo: if(Arrays.equals(memb.b, new byte[8])) then don't add,
			// and
			// warn user
		} catch (IOException e) {
			e.printStackTrace();
			IISUI.sysOut.atAlias.append("\n" + e.toString());
			// todo:?
		}
		memb.a = strans;
		IISUI.synonym.add(memb);
		IISUI.sysOut.atAlias.append("\n'" + memb.a + "' is added to synonym.");

		IISUI.tc.sels.set(IISUI.tc.ind, Utils.MacrRefrSelAs2ndLast(new SensSyn(memb)));
		IISUI.tc.sels.remove(IISUI.tc.ind + 1);
		IISUI.tc.Set2ndLastSel();
		IISUI.tc.CalcSelectionPos();
		IISUI.tc.DrawTextAndSelection();
	}

	@Override
	public void FuncSpacePress() {
		if (optans >= 0 && opts.get(optans) != null) {
			BI foundAgain = null;
			if (optsAlias.get(optans).isInverseVerb) {
				for (BI bi : IISUI.inverseverb)
					if (Arrays.equals(optsAlias.get(optans).syn.b, bi.b)) {
						foundAgain = bi;
						break;
					}
			} else {
				for (BI bi : IISUI.synonym)
					if (Arrays.equals(optsAlias.get(optans).syn.b, bi.b)) {
						foundAgain = bi;
						break;
					}
			}
			if (foundAgain == null || !foundAgain.a.equals(optsAlias.get(optans).syn.a)) {
				IISUI.sysOut.atAlias.append("\nThe option you selected has been modifed or deleted:");
				IISUI.sysOut.atAlias.append(optsAlias.get(optans));
				RefrSenseOptions();
				RefrSelAs2ndLast();
				IISUI.tc.DrawTextAndSelection();
				return;
			}
			if (optsAlias.get(optans).isInverseVerb)
				for (BI bi : IISUI.synonym)
					if (Arrays.equals(optsAlias.get(optans).syn.b, bi.b)) {
						foundAgain = bi;
						break;
					}
			IISUI.tc.sels.set(IISUI.tc.ind, Utils.MacrRefrSelAs2ndLast(Utils.GetSensDbacniOrSynFromSyn(foundAgain, null, foundAgain.b)));
			IISUI.tc.sels.remove(IISUI.tc.ind + 1);

			IISUI.tc.Set2ndLastSel();
			IISUI.tc.CalcSelectionPos();
			IISUI.tc.DrawTextAndSelection();
		}
	}
}
