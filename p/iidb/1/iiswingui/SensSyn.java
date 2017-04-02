package griddoor.iiswingui;

import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.util.Arrays;

import griddoor.iiswingui.IISUI.BI;
import griddoor.iiswingui.IISUI.IB;

public class SensSyn extends QueToAnsWithEitherOptOrStr {

	public SensSyn(BI syn) {
		super(" <SYN:" + syn.a + "> ");
		opts = Arrays.<CharSequence>asList("invertiblev", "noninvertib", "bytnoun", "dignoun", "conroot", "absroot", "mutexcl2", "mutexcl3", "subcate1", "subcate2", "inter2", "union2", "inter3", "union3", "relacomp2", "relacomp3" );
		this.syn = syn;
	}

	public BI syn;
	
	public IB MacrInsertIB(){
		IB memb = IISUI.theJfr.new IB();
		memb.a = syn.b;
		try {
			IISUI.outpStreSo.write(1);
			IISUI.outpStreSo.write(optans+1);
			IISUI.outpStreSo.write(memb.a);
			if (strans == null || strans.length() == 0) {
				IISUI.outpStreSo.write(0);
				IISUI.outpStreSo.write(0);
			} else {
				byte[] buf = strans.getBytes(StandardCharsets.UTF_8);
				IISUI.outpStreSo.write(buf.length % 256);
				IISUI.outpStreSo.write(buf.length / 256);
				IISUI.outpStreSo.write(buf);
				memb.b = strans;
			}
			if (IISUI.inpuStreSo.read() == 1) {
				// fixme: warn about failure
			}
			return memb;
		} catch (IOException e) {
			e.printStackTrace();
			IISUI.sysOut.atAlias.append("\n" + e.toString());
			// todo:?
			return null;
		}
	}

	@Override
	public void FuncWPress() {
		IB memb;
		switch (optans) {
		case 0:
			memb = MacrInsertIB();
			if(memb==null)
				break;
			IISUI.invertiblev.add(memb);
			IISUI.sysOut.atAlias.append("\n'" + syn.a + "' is added to invertiblev.");

			Utils.ReplSynWithUpgradedSens(syn.b, Utils.MacrRefrSelAs2ndLast(new SensInvertV(syn, "invertiblev")));
			
			break;
		case 1:
			memb = MacrInsertIB();
			if(memb==null)
				break;
			IISUI.noninvertib.add(memb);
			IISUI.sysOut.atAlias.append("\n'" + syn.a + "' is added to noninvertib.");

			Utils.ReplSynWithUpgradedSens(syn.b, Utils.MacrRefrSelAs2ndLast(new SensNoninvV(syn, "noninvertib")));
			
			break;
		case 2:
			memb = MacrInsertIB();
			if(memb==null)
				break;
			IISUI.bytnoun.add(memb);
			IISUI.sysOut.atAlias.append("\n'" + syn.a + "' is added to bytnoun.");

			Utils.ReplSynWithUpgradedSens(syn.b, Utils.MacrRefrSelAs2ndLast(new SensBytNoun(syn, "bytnoun")));
			break;
		case 3:
			memb = MacrInsertIB();
			if(memb==null)
				break;
			IISUI.dignoun.add(memb);
			IISUI.sysOut.atAlias.append("\n'" + syn.a + "' is added to dignoun.");

			Utils.ReplSynWithUpgradedSens(syn.b, Utils.MacrRefrSelAs2ndLast(new SensDigNoun(syn, "dignoun")));
			break;
		case 4:
			memb = MacrInsertIB();
			if(memb==null)
				break;
			IISUI.conroot.add(memb);
			IISUI.sysOut.atAlias.append("\n'" + syn.a + "' is added to conroot.");

			Utils.ReplSynWithUpgradedSens(syn.b, Utils.MacrRefrSelAs2ndLast(new SensConNoun(syn, "conroot")));
			break;
		case 5:
			memb = MacrInsertIB();
			if(memb==null)
				break;
			IISUI.absroot.add(memb);
			IISUI.sysOut.atAlias.append("\n'" + syn.a + "' is added to absroot.");

			Utils.ReplSynWithUpgradedSens(syn.b, Utils.MacrRefrSelAs2ndLast(new SensAbsNoun(syn, "absroot")));
			break;
		}
		IISUI.tc.Set2ndLastSel();
		IISUI.tc.CalcSelectionPos();
		IISUI.tc.DrawTextAndSelection();
		
	}

	@Override
	public void FuncSpacePress() {
		switch (optans) {
		case 6:
			break;
		case 7:
			break;
		case 8:
			Utils.RemoveAllOtherQueInThisLine(this);
			IISUI.tc.sels.addAll(IISUI.tc.ind, Arrays.asList(Utils.MacrRefrSelAs2ndLast(new SensSlot()),Utils.MacrRefrSelAs2ndLast(new SensCategory())));
			
			IISUI.tc.Set2ndLastSel();
			IISUI.tc.CalcSelectionPos();
			IISUI.tc.DrawTextAndSelection();
			
			
			
			//fixme: and maybe there are even relations between sense console
			// and clause console???!!! there are cases when you delete things
			// in the future
			break;
		case 9://undone
			break;
		case 10:
			break;
		case 11:
			break;
		case 12:
			break;
		case 13:
			break;
		case 14:
			Utils.RemoveAllOtherQueInThisLine(this);
			IISUI.tc.sels.addAll(IISUI.tc.ind, Arrays.asList(Utils.MacrRefrSelAs2ndLast(new SensSlot()),Utils.MacrRefrSelAs2ndLast(new SensCategory()),Utils.MacrRefrSelAs2ndLast(new SensSlot()),Utils.MacrRefrSelAs2ndLast(new SensCategory())));
			
			IISUI.tc.Set2ndLastSel();
			IISUI.tc.CalcSelectionPos();
			IISUI.tc.DrawTextAndSelection();
			break;
		case 15:
			break;
		}
	}
}
