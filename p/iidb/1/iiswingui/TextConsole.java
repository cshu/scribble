package griddoor.iiswingui;

import java.awt.event.KeyEvent;
import java.util.ArrayList;
import java.util.Arrays;

public abstract class TextConsole {
	public ArrayList<Selectable> sels;
	public int ind;

	public TextConsole() {
		sels = new ArrayList<Selectable>();
	}

	public int selStInd;
	public int selEnInd;

	public Selectable croppedSel1;
	public Selectable croppedSel2;

	public void Set2ndLastSel() {
		sels.set(sels.size() - 2, sels.get(ind).GetSelFor2ndLastSel());
	}

	public void CalcSelectionPos() {
		selStInd = 0;
		for (int i = 0; i < ind; i++)
			selStInd += sels.get(i).length();
		selEnInd = selStInd + sels.get(ind).length();
	}

	public void DrawTextAndSelection() {
		StringBuilder sb = new StringBuilder();
		for (Selectable sel : sels) {
			sb.append(sel);
		}
		IISUI.outJEP.setText(sb.toString());
		IISUI.outJEP.setSelectionStart(selStInd);
		IISUI.outJEP.setSelectionEnd(selEnInd);
	}

	public void AddNewlnAndQue_RefrSelAs2ndLast_Set2ndLastSel_CalcSelectionPos_DrawTextAndSelection(Selectable que) {
		ind += 2;
		sels.subList(IISUI.tc.ind, IISUI.tc.sels.size() - 2).clear();
		sels.addAll(IISUI.tc.ind, Arrays.asList(que, IISUI.newlineSel));
		sels.get(IISUI.tc.ind).RefrSelAs2ndLast();
		Set2ndLastSel();
		CalcSelectionPos();
		DrawTextAndSelection();
	}

	public void FuncKeyRelease(KeyEvent e) {
	}
}
