package griddoor.iiswingui;

import griddoor.util.UtilFuncs;

import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Comparator;

public class TextConsolePredicates extends TextConsoleQueList {

	public TextConsolePredicates() {
		title="PREDICATES SEARCH MODE (press E to refresh)";
	}

	public ArrayList<Pred> preds;

	@Override
	public ArrayList<? extends Selectable> getQues() {
		return preds;
	}

	public ArrayList<ArrayList<ClauRecord>> rawPreds = new ArrayList<ArrayList<ClauRecord>>();
	public ArrayList<byte[]> tabNames = new ArrayList<byte[]>();

	@Override
	public void FuncEPress() {
		rawPreds.clear();
		tabNames.clear();
		try {
			IISUI.outpStreSo.write(3);
			loaddata: for (;;) {
				ClauRecord cr = new ClauRecord(IISUI.inpuStreSo.read());
				byte[] tabname;
				switch (cr.typenum) {
				case 2:
					UtilFuncs.InputStreamReadBytesUntil(8, IISUI.inpuStreSo, cr.nseg1 = new byte[8], 0);
					UtilFuncs.InputStreamReadBytesUntil(8, IISUI.inpuStreSo, cr.vseg1 = new byte[8], 0);
					UtilFuncs.InputStreamReadBytesUntil(8, IISUI.inpuStreSo, cr.nseg2 = new byte[8], 0);
					break;
				case 17:
					UtilFuncs.InputStreamReadBytesUntil(8, IISUI.inpuStreSo, cr.nseg1 = new byte[8], 0);
					UtilFuncs.InputStreamReadBytesUntil(8, IISUI.inpuStreSo, cr.vseg1 = new byte[8], 0);
					UtilFuncs.InputStreamReadBytesUntil(8, IISUI.inpuStreSo, cr.vseg2 = new byte[8], 0);
					break;
				case 18:
					UtilFuncs.InputStreamReadBytesUntil(8, IISUI.inpuStreSo, cr.vseg1 = new byte[8], 0);
					UtilFuncs.InputStreamReadBytesUntil(8, IISUI.inpuStreSo, cr.vseg2 = new byte[8], 0);
					UtilFuncs.InputStreamReadBytesUntil(8, IISUI.inpuStreSo, cr.nseg1 = new byte[8], 0);
					break;
				case 19:
					UtilFuncs.InputStreamReadBytesUntil(8, IISUI.inpuStreSo, cr.nseg1 = new byte[8], 0);
					UtilFuncs.InputStreamReadBytesUntil(8, IISUI.inpuStreSo, cr.vseg1 = new byte[8], 0);
					UtilFuncs.InputStreamReadBytesUntil(8, IISUI.inpuStreSo, cr.nseg2 = new byte[8], 0);
					break;
				case 20:
					UtilFuncs.InputStreamReadBytesUntil(8, IISUI.inpuStreSo, cr.nseg1 = new byte[8], 0);
					UtilFuncs.InputStreamReadBytesUntil(8, IISUI.inpuStreSo, cr.vseg1 = new byte[8], 0);
					UtilFuncs.InputStreamReadBytesUntil(8, IISUI.inpuStreSo, cr.nseg2 = new byte[8], 0);
					break;
				case 3:
				case 5:
				case 7:
				case 14:
				case 16:
					UtilFuncs.InputStreamReadBytesUntil(8, IISUI.inpuStreSo, cr.nseg1 = new byte[8], 0);
					UtilFuncs.InputStreamReadBytesUntil(8, IISUI.inpuStreSo, cr.sense = new byte[8], 0);
					break;
				case 4:
					UtilFuncs.InputStreamReadBytesUntil(8, IISUI.inpuStreSo, cr.nseg1 = new byte[8], 0);
					UtilFuncs.InputStreamReadBytesUntil(8, IISUI.inpuStreSo, cr.sense = new byte[8], 0);
					UtilFuncs.InputStreamReadBytesUntil(8, IISUI.inpuStreSo, cr.insid = new byte[8], 0);
					break;
				case 6:
				case 8:
					UtilFuncs.InputStreamReadBytesUntil(8, IISUI.inpuStreSo, cr.nseg1 = new byte[8], 0);
					UtilFuncs.InputStreamReadBytesUntil(8, IISUI.inpuStreSo, cr.sense = new byte[8], 0);
					cr.digorbyt = new byte[IISUI.inpuStreSo.read() + IISUI.inpuStreSo.read() * 256];
					UtilFuncs.InputStreamReadBytesUntil(cr.digorbyt.length, IISUI.inpuStreSo, cr.digorbyt, 0);
					break;
				case 9:
				case 11:
					UtilFuncs.InputStreamReadBytesUntil(8, IISUI.inpuStreSo, cr.nseg1 = new byte[8], 0);
					break;
				case 10:
				case 12:
					UtilFuncs.InputStreamReadBytesUntil(8, IISUI.inpuStreSo, cr.nseg1 = new byte[8], 0);
					cr.digorbyt = new byte[IISUI.inpuStreSo.read() + IISUI.inpuStreSo.read() * 256];
					UtilFuncs.InputStreamReadBytesUntil(cr.digorbyt.length, IISUI.inpuStreSo, cr.digorbyt, 0);
					break;
				case 13:
				case 15:
					UtilFuncs.InputStreamReadBytesUntil(8, IISUI.inpuStreSo, cr.vseg1 = new byte[8], 0);
					UtilFuncs.InputStreamReadBytesUntil(8, IISUI.inpuStreSo, cr.sense = new byte[8], 0);
					break;
				case 255:
					break loaddata;
				}
				tabname = new byte[IISUI.inpuStreSo.read() + IISUI.inpuStreSo.read() * 256];
				UtilFuncs.InputStreamReadBytesUntil(tabname.length, IISUI.inpuStreSo, tabname, 0);
				for (int i = 0; i < tabNames.size(); i++)
					if (Arrays.equals(tabNames.get(i), tabname)) {
						rawPreds.get(i).add(cr);
						continue loaddata;
					}
				ArrayList<ClauRecord> crlist = new ArrayList<ClauRecord>();
				crlist.add(cr);
				rawPreds.add(crlist);
				tabNames.add(tabname);
			}
			if (IISUI.inpuStreSo.available() > 0)
				IISUI.sysOut.atAlias.append("\nGet data not over exception!");
		} catch (IOException e) {
			e.printStackTrace();
			IISUI.sysOut.atAlias.append("\n" + e.toString());
		}

		preds = new ArrayList<Pred>(rawPreds.size());
		for (ArrayList<ClauRecord> rp : rawPreds) {
			rp.sort(new Comparator<ClauRecord>(){
				@Override
				public int compare(ClauRecord o1, ClauRecord o2) {
					if(o1.nseg1==null)
						return 1;
					if(o2.nseg1==null)
						return -1;
					for(int j=7;j>=0;j--){
						int diff = o1.nseg1[j]-o2.nseg1[j];
						if(diff!=0)
							return diff;
					}
					return 0;
				}});
			preds.add(Pred.Make(rp));
		}
		
		quesOffset=0;
		ind = 1;
		RefrPrintedQues();
	}
}
