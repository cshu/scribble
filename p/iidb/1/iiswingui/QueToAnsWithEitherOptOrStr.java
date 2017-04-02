package griddoor.iiswingui;

import java.awt.event.KeyEvent;
import java.util.List;

public class QueToAnsWithEitherOptOrStr extends QueToAns {
	public QueToAnsWithEitherOptOrStr(CharSequence allText) {
		super(allText);
	}

	public List<? extends CharSequence> opts;
	public int optans = -1;
	public int optoffset = 0;

	public String strans;

	@Override
	public void GetStrToFillJta() {
		IISUI.textInput.setText(strans);
	}

	@Override
	public void FuncStrTraverseFromJta(String str) {
		strans = str;
		RefrSelAs2ndLast();
		IISUI.tc.DrawTextAndSelection();
	}

	@Override
	public void RefrSelAs2ndLast() {
		StringBuilder sb = new StringBuilder("STRING ANS AND OPTIONS:");
		if (strans != null) {
			sb.append('\n');
			sb.append(strans);
		}
		for (int i = 0; i + optoffset < opts.size() && i < 9; i++) {
			sb.append('\n');
			sb.append(i + 1);
			if (i + optoffset == optans)
				sb.append(" #### ");
			else
				sb.append("      ");
			sb.append(opts.get(i + optoffset));
		}
		if (opts.size() > 9) {
			sb.append('\n');
			sb.append(optoffset);
			sb.append('/');
			sb.append(opts.size());
		}
		selAs2ndLast.allText = sb.toString();
	}

	@Override
	public void FuncKeyPressed(KeyEvent e) {
		switch (e.getKeyCode()) {
		case KeyEvent.VK_SPACE:
			FuncSpacePress();
			break;
		case KeyEvent.VK_W:
			FuncWPress();
			break;
		case KeyEvent.VK_F:
			if (optoffset + 9 < opts.size())
				optoffset += 9;
			RefrSelAs2ndLast();
			IISUI.tc.DrawTextAndSelection();
			break;
		case KeyEvent.VK_B:
			if (optoffset != 0)
				optoffset -= 9;
			RefrSelAs2ndLast();
			IISUI.tc.DrawTextAndSelection();
			break;
		default:
			if (e.getKeyCode() >= KeyEvent.VK_1 && e.getKeyCode() <= KeyEvent.VK_9) {
				int tmp = e.getKeyCode() - KeyEvent.VK_1;
				if (tmp + optoffset >= opts.size())
					return;
				optans = tmp + optoffset;
				RefrSelAs2ndLast();
				IISUI.tc.DrawTextAndSelection();
			}
			break;
		}
	}

	public void FuncSpacePress() {
	}

	public void FuncWPress() {

	}
}
