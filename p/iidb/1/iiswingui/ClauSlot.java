package griddoor.iiswingui;

import java.util.Arrays;

public class ClauSlot extends QueToAnsWithOpt {

	public ClauSlot() {
		super(" [CLAUSE] ");
		opts = IISUI.listOfClauseTypes;
	}

	int optansSubmitted;
	int seqseg;
	
	@Override
	public void FuncSpacePress() {
		if (Utils.IsCsUsedAsSegInAnyClause(allText))
			return;
		IISUI.tcCla.SetNullInNOrVSegsIfExist(allText);
		switch(optans){
		case 1:
			allText = opts.get(optans) + " ";
			Utils.RemoveAllOtherQueInThisLine(this);//todo optimize removeQuesToTheRight
			IISUI.tc.ind++;
			IISUI.tc.sels.addAll(IISUI.tc.ind, Arrays.asList(Utils.MacrRefrSelAs2ndLast(new ClauNSegSlot()),Utils.MacrRefrSelAs2ndLast(new ClauVSegSlot()),Utils.MacrRefrSelAs2ndLast(new ClauNSegSlot())));

			IISUI.tc.Set2ndLastSel();
			break;
		case 2:
			seqseg = ++IISUI.tcCla.seqNSeg;
			allText = opts.get(optans) + " NSeg" + seqseg + " ";
			Utils.RemoveAllOtherQueInThisLine(this);//todo optimize removeQuesToTheRight
			IISUI.tc.ind++;
			IISUI.tc.sels.add(IISUI.tc.ind,Utils.MacrRefrSelAs2ndLast(new ClauConNounSensSlot()));

			IISUI.tc.Set2ndLastSel();
			break;
		case 3:
			//undone connounins
			break;
		case 4:
			seqseg = ++IISUI.tcCla.seqNSeg;
			allText = opts.get(optans) + " NSeg" + seqseg + " ";
			Utils.RemoveAllOtherQueInThisLine(this);//todo optimize removeQuesToTheRight
			IISUI.tc.ind++;
			IISUI.tc.sels.add(IISUI.tc.ind,Utils.MacrRefrSelAs2ndLast(new ClauBytNounSensSlot()));

			IISUI.tc.Set2ndLastSel();
			break;
		case 5:
			//undone bytnounins
			break;
		case 6:
			seqseg = ++IISUI.tcCla.seqNSeg;
			allText = opts.get(optans) + " NSeg" + seqseg + " ";
			Utils.RemoveAllOtherQueInThisLine(this);//todo optimize removeQuesToTheRight
			IISUI.tc.ind++;
			IISUI.tc.sels.add(IISUI.tc.ind,Utils.MacrRefrSelAs2ndLast(new ClauDigNounSensSlot()));

			IISUI.tc.Set2ndLastSel();
			break;
		case 7:
			//undone dignounins
			break;
		case 8:
		case 10:
			seqseg = ++IISUI.tcCla.seqNSeg;
			allText = opts.get(optans) + " NSeg" + seqseg + " ";
			Utils.RemoveAllOtherQueInThisLine(this);//todo optimize removeQuesToTheRight

			int i = IISUI.tcCla.nSegs.indexOf(null);
			if (i == -1)
				IISUI.tcCla.nSegs.add(allText);
			else
				IISUI.tcCla.nSegs.set(i, allText);
			IISUI.tcCla.nSegsVerno++;
			break;
		case 9:
			//undone rawbyteins
			break;
		case 11:
			//undone rawdigiins
			break;
		case 12:
		case 14:
			seqseg = ++IISUI.tcCla.seqVSeg;
			allText = opts.get(optans) + " VSeg" + seqseg + " ";
			Utils.RemoveAllOtherQueInThisLine(this);//todo optimize removeQuesToTheRight
			IISUI.tc.ind++;
			IISUI.tc.sels.add(IISUI.tc.ind,Utils.MacrRefrSelAs2ndLast(new ClauVerbSensSlot()));

			IISUI.tc.Set2ndLastSel();
			break;
		case 13:
		case 15:
			seqseg = ++IISUI.tcCla.seqNSeg;
			allText = opts.get(optans) + " NSeg" + seqseg + " ";
			Utils.RemoveAllOtherQueInThisLine(this);//todo optimize removeQuesToTheRight
			IISUI.tc.ind++;
			IISUI.tc.sels.add(IISUI.tc.ind,Utils.MacrRefrSelAs2ndLast(new ClauNounSensSlot()));

			IISUI.tc.Set2ndLastSel();
			break;
		case 16:
			allText = opts.get(optans) + " ";
			Utils.RemoveAllOtherQueInThisLine(this);//todo optimize removeQuesToTheRight
			IISUI.tc.ind++;
			IISUI.tc.sels.addAll(IISUI.tc.ind, Arrays.asList(Utils.MacrRefrSelAs2ndLast(new ClauNSegSlot()),Utils.MacrRefrSelAs2ndLast(new ClauVSegSlot()),Utils.MacrRefrSelAs2ndLast(new ClauVSegForAdveSlot())));

			IISUI.tc.Set2ndLastSel();
			break;
		case 17:
			allText = opts.get(optans) + " ";
			Utils.RemoveAllOtherQueInThisLine(this);//todo optimize removeQuesToTheRight
			IISUI.tc.ind++;
			IISUI.tc.sels.addAll(IISUI.tc.ind, Arrays.asList(Utils.MacrRefrSelAs2ndLast(new ClauVSegForAdveSlot()),Utils.MacrRefrSelAs2ndLast(new ClauVSegSlot()),Utils.MacrRefrSelAs2ndLast(new ClauNSegSlot())));

			IISUI.tc.Set2ndLastSel();
			break;
		case 18:
			allText = opts.get(optans) + " ";
			Utils.RemoveAllOtherQueInThisLine(this);//todo optimize removeQuesToTheRight
			IISUI.tc.ind++;
			IISUI.tc.sels.addAll(IISUI.tc.ind, Arrays.asList(Utils.MacrRefrSelAs2ndLast(new ClauNSegSlot()),Utils.MacrRefrSelAs2ndLast(new ClauVSegSlot()),Utils.MacrRefrSelAs2ndLast(new ClauNSegForRelaSlot())));

			IISUI.tc.Set2ndLastSel();
			break;
		case 19:
			allText = opts.get(optans) + " ";
			Utils.RemoveAllOtherQueInThisLine(this);//todo optimize removeQuesToTheRight
			IISUI.tc.ind++;
			IISUI.tc.sels.addAll(IISUI.tc.ind, Arrays.asList(Utils.MacrRefrSelAs2ndLast(new ClauNSegForRelaSlot()),Utils.MacrRefrSelAs2ndLast(new ClauVSegSlot()),Utils.MacrRefrSelAs2ndLast(new ClauNSegSlot())));

			IISUI.tc.Set2ndLastSel();
			break;
		}
		IISUI.tc.CalcSelectionPos();
		IISUI.tc.DrawTextAndSelection();
		optansSubmitted = optans;
	}
}
