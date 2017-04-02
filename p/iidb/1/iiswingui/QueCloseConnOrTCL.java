package griddoor.iiswingui;

import java.io.IOException;
import java.util.Arrays;

public class QueCloseConnOrTCL extends QueToAnsWithOpt {

	public QueCloseConnOrTCL() {
		super("Close/Commit/Commit+Begin Transaction?");
		opts = Arrays.<CharSequence>asList("Close connection", "Commit and then Begin transaction" );
	}

	@Override
	public void FuncSpacePress() {
		switch (optans) {
		case 0:
			try {
				IISUI.inpuStreSo.close();
				IISUI.outpStreSo.close();
				IISUI.clientSocket.close();
				IISUI.sysOut.atAlias.append("\nConnection Closed.");
				IISUI.tc.sels.subList(IISUI.tc.ind, IISUI.tc.ind + 2).clear();
				IISUI.tc.ind -= 2;
				IISUI.tc.Set2ndLastSel();
				IISUI.tc.CalcSelectionPos();
			} catch (IOException e) {
				IISUI.sysOut.atAlias.append("\n" + e.toString());
				e.printStackTrace();
			}
			IISUI.tc.DrawTextAndSelection();
			break;
		case 1:
			break;
		}
	}

}
