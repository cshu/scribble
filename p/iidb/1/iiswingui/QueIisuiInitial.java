package griddoor.iiswingui;

import java.util.Arrays;

public class QueIisuiInitial extends QueToAnsWithOpt {

	public QueIisuiInitial() {
		super("IISwingUI Initial Question");
		opts = Arrays.<CharSequence>asList( "Something about II data", "Something about files", "Start some program", "Exit (only possible when you are disconnected)","Browse MAL");
	}

	@Override
	public void FuncSpacePress() {
		switch (optans) {
		case 0:
			IISUI.tc.AddNewlnAndQue_RefrSelAs2ndLast_Set2ndLastSel_CalcSelectionPos_DrawTextAndSelection(new QueWhichIIData());
			break;
		case 1:
			IISUI.tc.AddNewlnAndQue_RefrSelAs2ndLast_Set2ndLastSel_CalcSelectionPos_DrawTextAndSelection(new QueWhichFiles());
			break;
		case 2:
			break;
		case 3:
			if (IISUI.clientSocket == null || IISUI.clientSocket.isClosed())
				System.exit(0);
			break;
		case 4:
			IISUI.tc.AddNewlnAndQue_RefrSelAs2ndLast_Set2ndLastSel_CalcSelectionPos_DrawTextAndSelection(new QueMALWhatToSearchFor());
			break;
		}
	}
}
