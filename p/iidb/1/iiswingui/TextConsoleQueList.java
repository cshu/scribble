package griddoor.iiswingui;

import java.awt.event.KeyEvent;
import java.util.ArrayList;
import java.util.Arrays;

public abstract class TextConsoleQueList extends TextConsole {

	public TextConsoleQueList() {
	}
	

	@Override
	public void FuncKeyRelease(KeyEvent e) {
		switch (e.getKeyCode()) {
		case KeyEvent.VK_ESCAPE:
			IISUI.tc = IISUI.tcBas;
			IISUI.tc.DrawTextAndSelection();
			break;
		case KeyEvent.VK_E:
			FuncEPress();
			break;
		case KeyEvent.VK_W:
			FuncWPress();
			break;
		case KeyEvent.VK_F:
			if(getQues()!=null && quesOffset+10<getQues().size()){
				quesOffset+=10;
				ind = 2;
				RefrPrintedQues();
			}
			break;
		case KeyEvent.VK_B:
			if(getQues()!=null && quesOffset>0){
				quesOffset-=10;
				if(quesOffset<0)
					quesOffset=0;
				ind = 2;
				RefrPrintedQues();
			}
			break;
		}
	}

	public void FuncWPress() {
	}


	public void FuncEPress() {
	}

	public String title;
	public abstract ArrayList<? extends Selectable> getQues();
	public int quesOffset;

	public void RefrPrintedQues(){
		sels.subList(2, sels.size()-2).clear();
		int endInd = Math.min(getQues().size(), quesOffset+10);
		for(int i=quesOffset;i<endInd;i++){
			sels.addAll(2+(i-quesOffset)*2,Arrays.asList(Utils.MacrRefrSelAs2ndLast(getQues().get(i)),IISUI.newlineSel));
		}
		sels.get(0).allText = title+' '+ quesOffset+'/'+getQues().size();

		Set2ndLastSel();
		CalcSelectionPos();
		DrawTextAndSelection();
	}
}
