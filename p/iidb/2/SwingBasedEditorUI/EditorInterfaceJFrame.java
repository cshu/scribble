package griddoor.SwingBasedEditorUI;

import griddoor.util.EnvironmentUtil;
import griddoor.util.StringPrepender;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Font;
import java.awt.HeadlessException;
import java.awt.KeyEventDispatcher;
import java.awt.KeyboardFocusManager;
import java.awt.event.KeyEvent;
import java.awt.event.WindowEvent;
import java.awt.event.WindowFocusListener;
import java.util.LinkedList;

import javax.swing.JEditorPane;
import javax.swing.JFrame;
import javax.swing.JTextArea;
import javax.swing.text.Caret;

public class EditorInterfaceJFrame extends JFrame {

	private static final long serialVersionUID = 1L;

	public final JEditorPane outJEP = new JEditorPane();
	public final JTextArea textInput = new JTextArea();

	public final UIContext context = new UIContext();
	public EditorInterfaceJFrame(String title) throws HeadlessException {
		super(title);
		setDefaultCloseOperation(JFrame.DO_NOTHING_ON_CLOSE);
		setExtendedState(JFrame.MAXIMIZED_BOTH);
		add(outJEP, BorderLayout.CENTER);
		add(textInput, BorderLayout.SOUTH);

		textInput.setLineWrap(true);
		outJEP.setFocusable(false);
		outJEP.setEditable(false);
		textInput.setEditable(false);
		outJEP.setFont(new Font("Unifont", Font.PLAIN, 16));
		textInput.setFont(new Font("Unifont", Font.PLAIN, 16));
		outJEP.setBackground(Color.DARK_GRAY);
		outJEP.setCaretColor(Color.WHITE);
		outJEP.setForeground(Color.WHITE);
		textInput.setBackground(Color.GRAY);
		textInput.setCaretColor(Color.WHITE);
		textInput.setForeground(Color.WHITE);
		KeyboardFocusManager manager = KeyboardFocusManager.getCurrentKeyboardFocusManager();
		manager.addKeyEventDispatcher(this.new GlobalKeyEventDispatcher());
		addWindowFocusListener(new WindowFocusListener() {
			@Override
			public void windowLostFocus(WindowEvent e) {
			}
			@Override
			public void windowGainedFocus(WindowEvent e) {
				releInter();
			}
		});
//		editorTextSb = new StringBuilder();
//		editorTextSb.append(' ');
		context.sp = new StringPrepender();
		context.editor = this;
		switchingPage = new TextTabPage(context) {
			@Override
			public void funcAfterFocus() {
				components.clear();
				for (TextTabPage tab : tabs) {
					components.add(new ButtonWithFixedText(this,tab.toString()){
						@Override
						public void spacePressed() {
							switchToTab(this.cg.cursorIndex);
							this.cg.context.redrawNeeded = true;
						};
					});
				}
				this.cursorIndex = components.size()-1;
			}
			@Override
			public void escPressed() {
				this.context.editor.currentTab = this.context.editor.tabs.getLast();
				this.context.redrawNeeded = true;
			}
		};
		setVisible(true);
	}

	public void addTabNSwitchToIt(TextTabPage page) {
		tabs.add(page);
		currentTab = page;
		context.redrawNeeded = true;
	}

	public final TextTabPage switchingPage; 
	public final LinkedList<TextTabPage> tabs = new LinkedList<>();
	public TextTabPage currentTab;//note this is always the LAST element in list (or switching page)

	public void switchToTab(int index) {
		currentTab = tabs.remove(index);
		tabs.add(currentTab);
	}

//	public boolean currentlyAppendingSelectedComp;

//	public void makeLeadingChars(StringBuilder sb) {
//		if (currentlyAppendingSelectedComp)
//			Globals.makeLeadingCharsForSelected(sb);
//		else
//			Globals.makeLeadingCharsForNotSelected(sb);
//	}

//	public int textForUpdEditorLineCount;
//	public final StringBuilder editorTextSb;

//	public void updateEditorText() {
//		//undone wait, you should pass numOfLines together with Sb, so the filler can check
//		//undone if it's already exceeding, unnecessary drawing can be avoided
//
//		editorTextSb.append(tabs.size()).append(" Tab(s) ");
//		editorTextSb.append(staBarStr);
//		if (editorTextSb.length() > Globals.maxWidthOfLine) {
//			//undone modify to make ellipsis
//		}
//		editorTextSb.append('\n');
//		textForUpdEditorLineCount = 1;
//		currentTab.FillByAppending();
//		if (textForUpdEditorLineCount > Globals.maxNumOfLines) {
//			//undone make ellipsis?
//		}
//		//undone
//		outJEP.setText(editorTextSb.toString());
//		editorTextSb.setLength(1);
//		textInput.setText(currentTab.components.get(currentTab.selectedIndex).getTooltip());
//	}

	public String staBarStr = "Disconnected";
	public String compileOp;
	public void drawEditorText(String... strs){
		if(!context.redrawNeeded)
			return;
		context.prependLineCount = 0;
		currentTab.fillByPrependingAndSetTooltip();
		context.sp.prepend('\n');
		for(String str:strs)
			context.sp.prepend(str);
		context.sp.prepend(compileOp);
		context.sp.prepend(staBarStr);//todo check exceeding maxWidthOfLine
		context.sp.prepend(" Tab(s) ").prepend(tabs.size()).prepend(' ');
		outJEP.setText(context.sp.toString());
		context.sp.clear();
		textInput.setText("");//textInput.setText(currentTab.components.get(currentTab.selectedIndex).getTooltip());
		context.redrawNeeded = false;
	}

	//note these are some methods that you considered but didn't adopt
	//	1.
	//	use map to store pressed keys
	//	release to remove
	//	2.
	//	getModifiersEx
	//	3.
	//	measure time gaps between presses
	//  4.
	//  hashset to add and remove keycode (this one doesn't work at all coz on linux release fires over and over when you hold down the key??)

	//  you have "safe keys", including all ascii keys and ESC, Caps Lock, Tab, F1-F12, Insert, Delete, Home, End, PageUp, PageDown, up, down, left, right
	//	once any other key is pressed, you enter lock mode, during which no input is accepted (with the exception of TEXTINPUT, lock mode doesn't affect typing in TEXTINPUT)
	//	(note TEXTINPUT has its own "safe scope", e.g. ALT and F10 must be intercepted. There might be more.)
	//	why are safe keys safe? coz for conventional OSs, any combination of these keys shouldn't trigger any OS-level behavior. (e.g. Ctrl+Esc, Alt+Tab, Win+E)
	//  AND these keys shouldn't be the first key to press when you want to do an OS-level combination key press!

	//	uses these keys for your actions, so you never worry about things like switching window can trigger your program's action

	//	Caps Lock is the SUPER SAFE key, exclusively for unlocking the locked UI. coz for conventional OSs, Caps Lock shouldn't be involved in any OS-level behavior!!
	//	Caps Lock is the only key suitable (okay, maybe also scroll/num lock) for unlocking. coz if you use another key for unlocking, isn't that key possibly involved in some OS-level behavior???

	//note the key_typed event cannot be intercepted by checking keycode

	//note why use key_pressed: coz release behavior differs on different OSs, so let's just use press and so the user won't have any reason to keep pressing a key, which might fire events over and over on linux.
	//note press behavior also differs on different OSs, on windows, press fires multiple times when you hold it.
	//note but still using press is better, cuz it's guaranteed to fire first, no trouble about keep pressing
	//note it's all about habit, if a user is allowed to keep holding a key, bad habit is nurtured, and then it might hurt you badly when you change OS and expect everything remains the same

	private boolean locked;
	private boolean visualElseInsertMode;//undone use a byte to represent 3 states: normal, search, bind?
	private void releInter() {
		locked = false;
		outJEP.setBackground(Color.DARK_GRAY);
	}
	private void lockInter() {
		locked = true;
		outJEP.setBackground(Color.GRAY);
	}
	private class GlobalKeyEventDispatcher implements KeyEventDispatcher {
		private boolean isLockedOrNotPresEv(KeyEvent e) {
			if (locked)
				return true;
			if (e.getModifiers() == 0 && e.getModifiersEx() == 0){
				if(e.getID() == KeyEvent.KEY_PRESSED)
					return false;
				else
					return true;
			}
			lockInter();
			return true;
		}
		private void  switchToNormalMode(){
			Caret caret = textInput.getCaret();
			caret.setSelectionVisible(false);
			caret.setVisible(false);
			textInput.setBackground(Color.GRAY);
			textInput.setEditable(false);
		}
		private void  switchToVisualMode(){
			Caret caret = textInput.getCaret();
			caret.setSelectionVisible(true);
			caret.setVisible(true);
			textInput.setBackground(Color.BLACK);
			textInput.setEditable(true);
			visualElseInsertMode = true;
		}
		private void  switchToInsertMode(){
			Caret caret = textInput.getCaret();
			caret.setSelectionVisible(true);
			caret.setVisible(true);
			textInput.setBackground(Color.BLUE);
			textInput.setEditable(true);
			visualElseInsertMode = false;
		}
		private String textInputBackup;//note this string is used to restore the field when esc is pressed. Without this string, you'll have to call redraw().

		@Override
		public boolean dispatchKeyEvent(KeyEvent e) {
			if (!java.awt.EventQueue.isDispatchThread())//debug
				System.out.println("NOT dispatching thread!!\n" + Thread.currentThread().getStackTrace()[0].toString());
			//change title, when everything is finished, change back
			String theTitle = getTitle();
			//undone e.getWhen() if more than 1 second ago, ignore it (but maybe you have a special key (like Insert/Delete) to disable this checking for one turn?
			context.uOrDJustPressed = false;
			switch (e.getKeyCode()) {
			//note! getmodifiersex is still used! to be safer cuz it might be possible that some key is already pressed before switching to UI. the locking might not fire.
			case KeyEvent.VK_CAPS_LOCK:
				releInter();
				return true;
			case KeyEvent.VK_0:
			case KeyEvent.VK_1:
			case KeyEvent.VK_2:
			case KeyEvent.VK_3:
			case KeyEvent.VK_4:
			case KeyEvent.VK_5:
			case KeyEvent.VK_6:
			case KeyEvent.VK_7:
			case KeyEvent.VK_8:
			case KeyEvent.VK_9:
			case KeyEvent.VK_A:
			case KeyEvent.VK_ADD:
				//case KeyEvent.VK_AMPERSAND:
				//case KeyEvent.VK_ASTERISK:
				//case KeyEvent.VK_AT:
			case KeyEvent.VK_B:
			case KeyEvent.VK_BACK_QUOTE:
			case KeyEvent.VK_BACK_SLASH:
			case KeyEvent.VK_BACK_SPACE:
				//case KeyEvent.VK_BRACELEFT:
				//case KeyEvent.VK_BRACERIGHT:
			case KeyEvent.VK_C:
				//case KeyEvent.VK_CIRCUMFLEX:
			case KeyEvent.VK_CLOSE_BRACKET:
				//case KeyEvent.VK_COLON:
			case KeyEvent.VK_COMMA:
				//case KeyEvent.VK_COPY://???????????????
				//case KeyEvent.VK_CUT://????????????
				//case KeyEvent.VK_DECIMAL://the del key???????
			case KeyEvent.VK_DELETE:
			case KeyEvent.VK_DIVIDE:
				//case KeyEvent.VK_DOLLAR:
				break;
			case KeyEvent.VK_DOWN:
				if(textInput.isEditable())
					return false;
				if(isLockedOrNotPresEv(e))
					return true;
//				for (int i = currentTab.selectedIndex + 1; i < currentTab.components.size(); i++) {
//					if (currentTab.components.get(i).isSelectable()) {
//						currentTab.selectedIndex = i;
//						drawEditorText();
//						break;
//					}
//				}
				currentTab.cursorMoveDown();
				drawEditorText();
				return true;
			case KeyEvent.VK_E:
			case KeyEvent.VK_END:
			case KeyEvent.VK_EQUALS:
				//case KeyEvent.VK_EXCLAMATION_MARK:
			case KeyEvent.VK_F:
			case KeyEvent.VK_F1:
			case KeyEvent.VK_F11:
			case KeyEvent.VK_F12:
			case KeyEvent.VK_F2:
				break;
			case KeyEvent.VK_ENTER:
				if(textInput.isEditable())
					return false;
				if(isLockedOrNotPresEv(e))
					return true;
				//undone call enterPress()
				//undone this key serves as the default confirm key for modal dialog, just like windowing system
				return true;
			case KeyEvent.VK_SPACE:
				if(textInput.isEditable())
					return false;
				if(isLockedOrNotPresEv(e))
					return true;
//				currentTab.components.get(currentTab.selectedIndex).spacePressed();
				setTitle("...");
				currentTab.spacePressed();
				drawEditorText();
				setTitle(theTitle);
				return true;
			case KeyEvent.VK_X:
				if(textInput.isEditable())
					return false;
				if(isLockedOrNotPresEv(e))
					return true;
				setTitle("...");
				currentTab.xPressed();
				drawEditorText();
				setTitle(theTitle);
				return true;
			case KeyEvent.VK_TAB:
				if(textInput.isEditable())
					return false;
				if(isLockedOrNotPresEv(e))
					return true;
				//undone show switching tab page
				currentTab = switchingPage;
				currentTab.funcAfterFocus();
				context.redrawNeeded = true;
				drawEditorText();
				return true;
				//undone show ellipsis after the last line when there are lines hidden. same goes for first line
			case KeyEvent.VK_D:
				if(textInput.isEditable())
					return false;
				if(isLockedOrNotPresEv(e))
					return true;
				context.uOrDJustPressed = true;
				currentTab.dPressed();
				drawEditorText();
				return true;
			case KeyEvent.VK_U:
				if(textInput.isEditable())
					return false;
				if(isLockedOrNotPresEv(e))
					return true;
				context.uOrDJustPressed = true;
				currentTab.uPressed();
				drawEditorText();
				return true;
			case KeyEvent.VK_N:
				if(textInput.isEditable())
					return false;
				if(isLockedOrNotPresEv(e))
					return true;
				//undone search next occurrence within the component GROUP!
				return true;
			case KeyEvent.VK_K://? maybe I don't need h and l??
				if(textInput.isEditable())
					return false;
				if(isLockedOrNotPresEv(e))
					return true;
//				for (int i = currentTab.selectedIndex - 1; i >= (currentTab.modalOffsets.isEmpty()?0:currentTab.modalOffsets.get(currentTab.modalOffsets.size()-1)); i--) {
//					if (currentTab.components.get(i).isSelectable()) {
//						currentTab.selectedIndex = i;
//						drawEditorText();
//						break;
//					}
//				}
				currentTab.cursorMoveUp();
				drawEditorText();
				return true;
			case KeyEvent.VK_J:
				if(textInput.isEditable())
					return false;
				if(isLockedOrNotPresEv(e))
					return true;
//				for (int i = currentTab.selectedIndex + 1; i < currentTab.components.size(); i++) {
//					if (currentTab.components.get(i).isSelectable()) {
//						currentTab.selectedIndex = i;
//						drawEditorText();
//						break;
//					}
//				}
				currentTab.cursorMoveDown();
				drawEditorText();
				return true;
			case KeyEvent.VK_F3:
				if(textInput.isEditable())
					return false;
				if(isLockedOrNotPresEv(e))
					return true;
				switchToVisualMode();
				textInputBackup = textInput.getText();
				textInput.setText("");//todo use last query?
				return true;
			case KeyEvent.VK_F6:
				if(textInput.isEditable())
					return false;
				if(isLockedOrNotPresEv(e))
					return true;
//				if(currentTab.components.get(currentTab.selectedIndex) instanceof JavaStringTextArea){
				String strToEdit = currentTab.strForInsertMode();
				if(strToEdit!=null){
					switchToInsertMode();
					textInputBackup = textInput.getText();
//					textInput.setText(currentTab.components.get(currentTab.selectedIndex).getJavaStringFromThisAsTextArea());
					textInput.setText(strToEdit);
					textInput.selectAll();
				}
				return true;
			case KeyEvent.VK_F7:
				if(isLockedOrNotPresEv(e))
					return true;
				if(textInput.isEditable()){
					if(visualElseInsertMode)
						return true;//note bind key is only for insert mode, not visual mode
					switchToNormalMode();
//					currentTab.components.get(currentTab.selectedIndex).bindStrFunc();
					setTitle("...");
					currentTab.bindStrFunct();
					drawEditorText();
					setTitle(theTitle);
				}
				return true;
			case KeyEvent.VK_ESCAPE:
				if(isLockedOrNotPresEv(e))
					return true;
				if(textInput.isEditable()){
					switchToNormalMode();
					textInput.setText(textInputBackup);
					//undone clear search results' markings? (may be BOLD, COLOR, ENDING CHAR?)
				}else{
					setTitle("...");
					currentTab.escPressed();
					drawEditorText();
					setTitle(theTitle);
				}
				return true;
			case KeyEvent.VK_F4:
			case KeyEvent.VK_F5:
			case KeyEvent.VK_F8:
			case KeyEvent.VK_F9:
			case KeyEvent.VK_G:
				//case KeyEvent.VK_GREATER:
			case KeyEvent.VK_H:
			case KeyEvent.VK_HOME:
			case KeyEvent.VK_I:
			case KeyEvent.VK_INSERT:
				//case KeyEvent.VK_INVERTED_EXCLAMATION_MARK:
			case KeyEvent.VK_KP_DOWN:
			case KeyEvent.VK_KP_LEFT:
			case KeyEvent.VK_KP_RIGHT:
			case KeyEvent.VK_KP_UP:
			case KeyEvent.VK_L:
			case KeyEvent.VK_LEFT:
				//case KeyEvent.VK_LEFT_PARENTHESIS:
				//case KeyEvent.VK_LESS:
			case KeyEvent.VK_M:
			case KeyEvent.VK_MINUS:
			case KeyEvent.VK_MULTIPLY:
			case KeyEvent.VK_NUM_LOCK:
				//case KeyEvent.VK_NUMBER_SIGN:
			case KeyEvent.VK_NUMPAD0:
			case KeyEvent.VK_NUMPAD1:
			case KeyEvent.VK_NUMPAD2:
			case KeyEvent.VK_NUMPAD3:
			case KeyEvent.VK_NUMPAD4:
			case KeyEvent.VK_NUMPAD5:
			case KeyEvent.VK_NUMPAD6:
			case KeyEvent.VK_NUMPAD7:
			case KeyEvent.VK_NUMPAD8:
			case KeyEvent.VK_NUMPAD9:
			case KeyEvent.VK_O:
			case KeyEvent.VK_OPEN_BRACKET:
			case KeyEvent.VK_P:
				break;
			case KeyEvent.VK_PAGE_DOWN:
				if(textInput.isEditable())
					return false;
				if(isLockedOrNotPresEv(e))
					return true;
				context.uOrDJustPressed = true;
				currentTab.dPressed();
				drawEditorText();
				return true;
			case KeyEvent.VK_PAGE_UP:
				if(textInput.isEditable())
					return false;
				if(isLockedOrNotPresEv(e))
					return true;
				context.uOrDJustPressed = true;
				currentTab.uPressed();
				drawEditorText();
				return true;
				//case KeyEvent.VK_PASTE://what the hell?? does ctrl v trigger this??
			case KeyEvent.VK_PERIOD:
				//case KeyEvent.VK_PLUS:
				//case KeyEvent.VK_PROPS:
			case KeyEvent.VK_Q:
			case KeyEvent.VK_QUOTE:
				//case KeyEvent.VK_QUOTEDBL:
			case KeyEvent.VK_R:
			case KeyEvent.VK_RIGHT:
				//case KeyEvent.VK_RIGHT_PARENTHESIS:
			case KeyEvent.VK_S:
			case KeyEvent.VK_SCROLL_LOCK:
			case KeyEvent.VK_SEMICOLON:
			case KeyEvent.VK_SLASH:
			case KeyEvent.VK_SUBTRACT:
			case KeyEvent.VK_T:
				//case KeyEvent.VK_UNDERSCORE:
				break;
			case KeyEvent.VK_UP:
				if(textInput.isEditable())
					return false;
				if(isLockedOrNotPresEv(e))
					return true;
//				for (int i = currentTab.selectedIndex - 1; i >= (currentTab.modalOffsets.isEmpty()?0:currentTab.modalOffsets.get(currentTab.modalOffsets.size()-1)); i--) {
//					if (currentTab.components.get(i).isSelectable()) {
//						currentTab.selectedIndex = i;
//						drawEditorText();
//						break;
//					}
//				}
				currentTab.cursorMoveUp();
				drawEditorText();
				return true;
			case KeyEvent.VK_V:
			case KeyEvent.VK_W:
				break;
			case KeyEvent.VK_Y:
				if(textInput.isEditable())
					return false;
				if(isLockedOrNotPresEv(e))
					return true;
				//undone what about user only wants to copy the selected line.
				//undone what about user want ALL text including the part outside the visible window (exceeding max num of lines)
				EnvironmentUtil.setStringToSystemClipboard(outJEP.getText());
				return true;
			case KeyEvent.VK_Z:
				break;
			case KeyEvent.VK_ALT:
				lockInter();
			case KeyEvent.VK_F10:
				return true;
			case KeyEvent.VK_UNDEFINED:
				if (e.getID() == KeyEvent.KEY_TYPED)
					break;
			default:
				lockInter();
				break;
			}
			if (textInput.isEditable())
				return false;
			return true;
		}
	}
	//undone this part is so confusing, you probably need to test it on different OSs

}
