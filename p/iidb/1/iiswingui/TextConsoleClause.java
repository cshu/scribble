package griddoor.iiswingui;

import java.awt.event.KeyEvent;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;

public class TextConsoleClause extends TextConsole {

	public TextConsoleClause() {
		ind = 2;
	}

	@Override
	public void FuncKeyRelease(KeyEvent e) {
		switch (e.getKeyCode()) {
		case KeyEvent.VK_ESCAPE:
			IISUI.tc = IISUI.tcBas;
			IISUI.tc.DrawTextAndSelection();
			break;
		case KeyEvent.VK_O:
			if (ind + 2 >= sels.size())
				return;
			while (sels.get(ind) != IISUI.newlineSel)
				ind++;
			sels.addAll(++ind, Arrays.asList(Utils.MacrRefrSelAs2ndLast(new ClauSlot()), IISUI.newlineSel));
			Set2ndLastSel();
			CalcSelectionPos();
			DrawTextAndSelection();
			break;
		case KeyEvent.VK_D:
			if (!(sels.get(ind) instanceof QueToAns))
				return;
			int prevNewlnInd = Utils.prevNewlnInd();
			CharSequence seg = sels.get(prevNewlnInd + 1).allText;
			if (Utils.IsCsUsedAsSegInAnyClause(seg))
				return;
			SetNullInNOrVSegsIfExist(seg);
			sels.subList(prevNewlnInd, Utils.nextNewlnInd()).clear();
			ind = prevNewlnInd;

			Set2ndLastSel();
			CalcSelectionPos();
			DrawTextAndSelection();
			break;
		case KeyEvent.VK_W:
			// fixme:
			// check to ensure there is no empty slot, else reject
			// get all clauses, when any 2 of them share common seg, mark them
			// in same group, it ends with more than 1 group, reject
			// on the basis of the above step, mark segments used in clauses, if
			// there is any left out, reject
			//
			ArrayList<int[]> clauses = new ArrayList<int[]>();
			ArrayList<int[]> segdata = new ArrayList<int[]>();
			ArrayList<int[]> segtype = new ArrayList<int[]>();
			HashMap<CharSequence, Integer> segtext = new HashMap<CharSequence, Integer>();
			for (int indOfSels = 0; indOfSels < sels.size(); indOfSels++) {
				Selectable sel = sels.get(indOfSels);
				if (sel instanceof ClauSlot) {
					ClauSlot alias = (ClauSlot) sel;
					if (alias.seqseg == 0)
						continue;
					segtext.put(alias.allText, alias.seqseg);
					segtype.add(new int[]{alias.seqseg, alias.optansSubmitted});
					segdata.add(new int[]{alias.seqseg, indOfSels});
				}
			}
			for (int indOfSels = 0; indOfSels < sels.size(); indOfSels++) {
				Selectable sel = sels.get(indOfSels);
				if (sel instanceof ClauSlot) {
					ClauSlot alias = (ClauSlot) sel;
					if (alias.seqseg == 0) {
						clauses.add(new int[] { alias.optansSubmitted, segtext.get(sels.get(indOfSels + 1).allText), segtext.get(sels.get(indOfSels + 2).allText), segtext.get(sels.get(indOfSels + 3).allText) });
					}
				}
			}

			ByteBuffer bbbuf = ByteBuffer.allocate(Integer.BYTES).order(ByteOrder.LITTLE_ENDIAN);
			try {
				IISUI.outpStreSo.write(2);

				for (int segti=0;segti<segtype.size();segti++) {
					int [] segt = segtype.get(segti);
					IISUI.outpStreSo.write(segt[1] + 1);
					IISUI.outpStreSo.write(bbbuf.putInt(segt[0]).array());
					bbbuf.rewind();
					IISUI.outpStreSo.write(0);
					IISUI.outpStreSo.write(0);
					IISUI.outpStreSo.write(0);
					IISUI.outpStreSo.write(0);
					switch(segt[1]){
					case 2:
					case 4:
					case 6:
					case 12:
					case 13:
					case 14:
					case 15:
						IISUI.outpStreSo.write(((ClauSensSlot)sels.get(segdata.get(segti)[1]+1)).syn.b);
						break;
					case 3:
					case 5:
					case 7:
						//undone connounins, bytnounins, dignounins
						break;
					case 8:
					case 10:
						//nothing to send
						break;
					case 9:
					case 11:
						//undone rawbyteins, rawdigiins
						break;
					}
				}
				for (int[] ia : clauses) {
					boolean isFirstByte = true;
					for (int i : ia) {
						if (isFirstByte) {
							IISUI.outpStreSo.write(i + 1);
							isFirstByte = false;
						} else {
							IISUI.outpStreSo.write(bbbuf.putInt(i).array());
							bbbuf.rewind();
							IISUI.outpStreSo.write(0);
							IISUI.outpStreSo.write(0);
							IISUI.outpStreSo.write(0);
							IISUI.outpStreSo.write(0);
						}
					}
				}
				IISUI.outpStreSo.write(255);

			} catch (IOException e1) {
				e1.printStackTrace();
				IISUI.sysOut.atAlias.append("\n" + e.toString());
				// todo:?
			}
			IISUI.sysOut.atAlias.append("\nPredicate is created.");

			IISUI.tc.DrawTextAndSelection();
			break;
		}
	}

	public int seqNSeg;
	public int seqVSeg;

	public ArrayList<CharSequence> nSegs = new ArrayList<CharSequence>();
	public ArrayList<CharSequence> vSegs = new ArrayList<CharSequence>();
	public int nSegsVerno;
	public int vSegsVerno;

	public void SetNullInNOrVSegsIfExist(CharSequence seg) {
		int indOfAt = nSegs.indexOf(seg);
		if (indOfAt == -1) {
			indOfAt = vSegs.indexOf(seg);
			if (indOfAt != -1) {
				vSegs.set(indOfAt, null);
				vSegsVerno++;
			}
		} else {
			nSegs.set(indOfAt, null);
			nSegsVerno++;
		}
	}
}
