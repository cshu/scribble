package griddoor.iiswingui;

import java.io.IOException;
import java.util.Arrays;

import griddoor.iiswingui.IISUI.BI;
import griddoor.iiswingui.IISUI.II;
import griddoor.iiswingui.IISUI.III;

public abstract class SensDbacni extends QueToAnsWithOpt {

	public BI syn;

	public SensDbacni(CharSequence allText, BI syn) {
		super(allText);
		opts = Arrays.<CharSequence> asList("view/update details", "use it in mutexcl2", "use it in mutexcl3", "use it in subcate1", "use it in subcate2", "use it in inter2", "use it in union2", "use it in inter3", "use it in union3", "use it in relacomp2", "use it in relacomp3");
		this.syn = syn;
	}
	
	public abstract SensDbacni MakeSensX(BI syn, CharSequence directIdTable);
	public abstract boolean instanceofT(Selectable sel);
	
	@Override
	public void FuncSPress() {
		//undone
		//send request to search all predicates containing syn.b
		//call a Macr of converting the received clauses/segs into predicates selectables
	}

	@Override
	public void FuncWPress() {
		switch (optans) {
		case 3:
			if (IISUI.tc.sels.get(IISUI.tc.ind - 1) == IISUI.newlineSel) {
				if (IISUI.tc.sels.get(IISUI.tc.ind + 2) == IISUI.newlineSel) {
					if (IISUI.tc.sels.get(IISUI.tc.ind + 1) instanceof SensSyn) {
						BI bSyn = ((SensSyn) IISUI.tc.sels.get(IISUI.tc.ind + 1)).syn;
						MacrInsertIntoSubcate1(syn.b, bSyn.b, syn.a, bSyn.a);
						Utils.ReplSynWithUpgradedSens(bSyn.b, Utils.MacrRefrSelAs2ndLast(MakeSensX(bSyn, "subcate1")));
						IISUI.tc.CalcSelectionPos();
						IISUI.tc.DrawTextAndSelection();
					} else if (instanceofT(IISUI.tc.sels.get(IISUI.tc.ind + 1))) {
						BI bSyn = ((SensDbacni) IISUI.tc.sels.get(IISUI.tc.ind + 1)).syn;
						// fixme: check tautology/any possible contradiction
						MacrInsertIntoSubcate1(syn.b, bSyn.b, syn.a, bSyn.a);
						IISUI.tc.DrawTextAndSelection();
					}
				}
			} else if (IISUI.tc.sels.get(IISUI.tc.ind + 1) == IISUI.newlineSel) {
				if (IISUI.tc.sels.get(IISUI.tc.ind - 2) == IISUI.newlineSel) {
					if (instanceofT(IISUI.tc.sels.get(IISUI.tc.ind - 1) )) {
						BI aSyn = ((SensDbacni) IISUI.tc.sels.get(IISUI.tc.ind - 1)).syn;
						// fixme: check tautology/any possible contradiction
						MacrInsertIntoSubcate1(aSyn.b, syn.b, aSyn.a, syn.a);
						IISUI.tc.DrawTextAndSelection();
					}
				}
			}
			break;
		case 5:
			int i = Utils.prevNewlnInd()+1;
			if(!(instanceofT(IISUI.tc.sels.get(i+1)) && instanceofT(IISUI.tc.sels.get(i+2)) && IISUI.tc.sels.get(i+3) == IISUI.newlineSel))
				return;
			BI bSyn = ((SensDbacni) IISUI.tc.sels.get(i+1)).syn;
			BI cSyn = ((SensDbacni) IISUI.tc.sels.get(i+2)).syn;
			if(instanceofT(IISUI.tc.sels.get(i))){
				BI aSyn = ((SensDbacni) IISUI.tc.sels.get(i)).syn;
				// fixme: check tautology/any possible contradiction
				MacrInsertIntoInter2(aSyn.b, bSyn.b, cSyn.b, aSyn.a, bSyn.a, cSyn.a);
				IISUI.tc.DrawTextAndSelection();
			} else if (IISUI.tc.sels.get(i) instanceof SensSyn){
				BI aSyn = ((SensSyn) IISUI.tc.sels.get(i)).syn;
				// fixme: check tautology/any possible contradiction
				MacrInsertIntoInter2(aSyn.b, bSyn.b, cSyn.b, aSyn.a, bSyn.a, cSyn.a);
				Utils.ReplSynWithUpgradedSens(aSyn.b, Utils.MacrRefrSelAs2ndLast(MakeSensX(aSyn, "inter2")));
				IISUI.tc.CalcSelectionPos();
				IISUI.tc.DrawTextAndSelection();
			}
			break;
		}
	}

	@Override
	public void FuncSpacePress() {
		switch (optans) {
		case 0:// undone
			break;
		case 1:
			break;
		case 2:
			break;
		case 3:
			Utils.RemoveAllOtherQueInThisLine(this);
			IISUI.tc.sels.addAll(IISUI.tc.ind + 1, Arrays.asList(Utils.MacrRefrSelAs2ndLast(new SensSlot()), Utils.MacrRefrSelAs2ndLast(new SensCategory())));

			IISUI.tc.CalcSelectionPos();
			IISUI.tc.DrawTextAndSelection();
			break;
		case 5:
			Utils.RemoveAllOtherQueInThisLine(this);
			IISUI.tc.sels.addAll(IISUI.tc.ind, Arrays.asList(Utils.MacrRefrSelAs2ndLast(new SensSlot()), Utils.MacrRefrSelAs2ndLast(new SensCategory()), Utils.MacrRefrSelAs2ndLast(new SensSlot()), Utils.MacrRefrSelAs2ndLast(new SensCategory())));
			IISUI.tc.ind += 4;

			IISUI.tc.CalcSelectionPos();
			IISUI.tc.DrawTextAndSelection();
			break;
		case 9:
			Utils.RemoveAllOtherQueInThisLine(this);
			IISUI.tc.sels.addAll(IISUI.tc.ind + 1, Arrays.asList(Utils.MacrRefrSelAs2ndLast(new SensSlot()), Utils.MacrRefrSelAs2ndLast(new SensCategory()), Utils.MacrRefrSelAs2ndLast(new SensSlot()), Utils.MacrRefrSelAs2ndLast(new SensCategory())));

			IISUI.tc.CalcSelectionPos();
			IISUI.tc.DrawTextAndSelection();
			break;
		}
	}

	public void MacrInsertIntoSubcate1(byte[] a, byte[] b, String aSp, String bSp) {
		try {
			IISUI.outpStreSo.write(1);
			IISUI.outpStreSo.write(9);
			IISUI.outpStreSo.write(a);
			IISUI.outpStreSo.write(b);
			if (IISUI.inpuStreSo.read() == 1) {
				// fixme: warn about failure
			}
		} catch (IOException e) {
			e.printStackTrace();
			IISUI.sysOut.atAlias.append("\n" + e.toString());
			// todo:?
		}

		II memb = IISUI.theJfr.new II();
		memb.a = a;
		memb.b = b;
		IISUI.subcate1.add(memb);
		IISUI.sysOut.atAlias.append("\n'" + aSp + "'|'" + bSp + "' is added to subcate1.");
	}

	public void MacrInsertIntoInter2(byte[] a, byte[] b, byte[] c, String aSp, String bSp, String cSp) {
		try {
			IISUI.outpStreSo.write(1);
			IISUI.outpStreSo.write(11);
			IISUI.outpStreSo.write(a);
			IISUI.outpStreSo.write(b);
			IISUI.outpStreSo.write(c);
			if (IISUI.inpuStreSo.read() == 1) {
				// fixme: warn about failure
			}
		} catch (IOException e) {
			e.printStackTrace();
			IISUI.sysOut.atAlias.append("\n" + e.toString());
			// todo:?
		}

		III memb = IISUI.theJfr.new III();
		memb.a = a;
		memb.b = b;
		memb.c = c;
		IISUI.inter2.add(memb);
		IISUI.sysOut.atAlias.append("\n'" + aSp + "'|'" + bSp + "'|'" + cSp + "' is added to inter2.");
	}
}
