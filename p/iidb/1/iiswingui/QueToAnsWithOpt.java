package griddoor.iiswingui;

import java.awt.event.KeyEvent;
import java.util.List;

public class QueToAnsWithOpt extends QueToAns {

	public QueToAnsWithOpt(CharSequence allText) {
		super(allText);
	}

	public List<? extends CharSequence> opts;
	public int optans = -1;
	public int optoffset = 0;
	public StringBuilder s2Alias;

	@Override
	public void RefrSelAs2ndLast() {
		s2Alias = new StringBuilder("OPTIONS:");
		for (int i = 0; i + optoffset < opts.size() && i < 9; i++) {
			s2Alias.append('\n');
			s2Alias.append(i + 1);
			if (i + optoffset == optans)
				s2Alias.append(" #### ");
			else
				s2Alias.append("      ");
			s2Alias.append(opts.get(i + optoffset));
		}
		if (opts.size() > 9) {
			s2Alias.append('\n');
			s2Alias.append(optoffset);
			s2Alias.append('/');
			s2Alias.append(opts.size());
		}
		selAs2ndLast.allText = s2Alias;//fixme: maybe it's okay to use stringbuilder?
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
		case KeyEvent.VK_S:
			FuncSPress();
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
	
	public void FuncSPress() {
	}
}
