package griddoor.SwingBasedEditorUI;

import griddoor.util.StringPrepender;

public class UIContext {
	public int prependLineCount;
	public StringPrepender sp;
	public char charToUseForCursorLine;//note using global rather than local, for smaller mem footprint
	public char charToUseForLine = ' ';
	public boolean redrawNeeded;
	public EditorInterfaceJFrame editor;
	public boolean uOrDJustPressed;
}
