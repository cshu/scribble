package griddoor.iiswingui;

import java.awt.event.KeyEvent;
import java.util.Arrays;

public class TextConsoleSense extends TextConsole {

	public TextConsoleSense() {
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
			if(ind+2>=sels.size())
				return;
			while(sels.get(ind)!=IISUI.newlineSel)
				ind++;
			sels.addAll(++ind, Arrays.asList(Utils.MacrRefrSelAs2ndLast(new SensSlot()),Utils.MacrRefrSelAs2ndLast(new SensCategory()),IISUI.newlineSel));
			Set2ndLastSel();
			CalcSelectionPos();
			DrawTextAndSelection();
			break;
		case KeyEvent.VK_X:
			if(ind+2>=sels.size())
				return;
			sels.get(ind).FuncXReleasedInTCSens();
			break;
		case KeyEvent.VK_P:
			sels.get(ind).FuncPReleasedInTCSens();
			break;
		case KeyEvent.VK_D:
			if(!(sels.get(ind) instanceof QueToAns))
				return;
			int prevNewlnInd = Utils.prevNewlnInd();
			sels.subList(prevNewlnInd,Utils.nextNewlnInd()).clear();;
			ind = prevNewlnInd;

			Set2ndLastSel();
			CalcSelectionPos();
			DrawTextAndSelection();
			break;
		}
	}
	
	
	public QueToAns synOrSenseCropped;

}
