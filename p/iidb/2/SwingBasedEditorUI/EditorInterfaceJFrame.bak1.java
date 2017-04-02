package griddoor.SwingBasedEditorUI;



import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Font;
import java.awt.HeadlessException;
import java.awt.KeyEventDispatcher;
import java.awt.KeyboardFocusManager;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.util.ArrayList;
import java.util.LinkedList;

import javax.swing.JEditorPane;
import javax.swing.JFrame;
import javax.swing.JTextArea;
import javax.swing.text.Caret;

public class EditorInterfaceJFrame extends JFrame {

	private static final long serialVersionUID = 1L;

	public final JEditorPane outJEP = new JEditorPane();
	public final JTextArea textInput = new JTextArea();
	public EditorInterfaceJFrame(String title) throws HeadlessException {
		super(title);
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);//undone remove
		//setDefaultCloseOperation(JFrame.DO_NOTHING_ON_CLOSE);
		setExtendedState(JFrame.MAXIMIZED_BOTH);
		add(outJEP, BorderLayout.CENTER);
		add(textInput, BorderLayout.SOUTH);
		
		outJEP.setFocusable(false);
		outJEP.setEditable(false);
		textInput.setEditable(false);
		outJEP.setFont(new Font("Consolas", Font.PLAIN, 15));
		textInput.setFont(new Font("Consolas", Font.PLAIN, 15));
		outJEP.setBackground(Color.GRAY);
		outJEP.setCaretColor(Color.WHITE);
		outJEP.setForeground(Color.WHITE);
		textInput.setBackground(Color.GRAY);
		textInput.setCaretColor(Color.WHITE);
		textInput.setForeground(Color.WHITE);
		KeyboardFocusManager manager = KeyboardFocusManager.getCurrentKeyboardFocusManager();
		manager.addKeyEventDispatcher(this.new GlobalKeyEventDispatcher());
		editorTextSb = new StringBuilder();
		Globals.makeLeadingCharsForNotSelected(editorTextSb);
		setVisible(true);
	}
	
	public void addTabNSwitchToIt(TextTabPage page){
		tabs.add(page);
		currentTab = page;
	}
	
	public final TextTabPage switchingPage = new TextTabPage(this){
		public void funcAfterFocus(){
			components.clear();
			for(TextTabPage tab : tabs){
				//components.add(btnForSwitchingPage);//undone
			}
		}
	};
	public final LinkedList<TextTabPage> tabs = new LinkedList<TextTabPage>();
	public TextTabPage currentTab;//note this is always the LAST element in list (or switching page)
	
	public void switchToTab(int index){
		currentTab = tabs.remove(index);
		tabs.add(currentTab);
	}
	
	public boolean currentlyAppendingSelectedComp;
	public void makeLeadingChars(StringBuilder sb){
		if(currentlyAppendingSelectedComp)
			Globals.makeLeadingCharsForSelected(sb);
		else
			Globals.makeLeadingCharsForNotSelected(sb);
	}
	
	public int textForUpdEditorLineCount;
	public final StringBuilder editorTextSb;

	public void updateEditorText(){
		
		
		editorTextSb.append(tabs.size()).append(" Tab(s)");
		//undone append status bar on the same line
		if(editorTextSb.length()>Globals.maxWidthOfLine){
			//undone modify to make ellipsis
		}
		editorTextSb.append('\n');
		textForUpdEditorLineCount = 1;
		int numOfLines = 1+currentTab.FillByAppending(editorTextSb);
		if(numOfLines>Globals.maxNumOfLines){
			//undone make ellipsis?
		}
		//undone
		outJEP.setText(editorTextSb.toString());
		editorTextSb.setLength(1);
	}


	
    private class GlobalKeyEventDispatcher implements KeyEventDispatcher {
    	private boolean locked;
		@Override
		public boolean dispatchKeyEvent(KeyEvent e) {
			switch(e.getKeyCode()){
			case KeyEvent.VK_ESCAPE:
				break;
			case KeyEvent.VK_CAPS_LOCK:
				break;
			default:
				
				break;
			}
			if(e.getID()==KeyEvent.KEY_PRESSED)
				System.out.println("press");
			else if(e.getID()==KeyEvent.KEY_TYPED)
				System.out.println("type");
			else if(e.getID()==KeyEvent.KEY_RELEASED)
				System.out.println("release");
			System.out.println(e.getModifiersEx());
			if(true)
				return true;
			if(!java.awt.EventQueue.isDispatchThread())//debug
				System.out.println("NOT dispatching thread!!\n"+Thread.currentThread().getStackTrace()[0].toString());
			if(e.isAltDown()||e.isAltGraphDown()||e.isControlDown()||e.isMetaDown()||e.isShiftDown())
				return true;
			Caret caret;
			if(textInput.isEditable()){
				switch(e.getKeyCode()){//note no need to check if(e.getID()==KeyEvent.KEY_PRESSED), KEY_PRESSED should come first
				case KeyEvent.VK_E:
					try {
						System.out.println("sleep started");
						Thread.sleep(6000);
						System.out.println("sleep ended");
					} catch (InterruptedException e1) {
						// TODO Auto-generated catch block
						e1.printStackTrace();
					}
					return true;
				case KeyEvent.VK_ALT:
					//undone confirm the input for search/binding
				case KeyEvent.VK_ESCAPE:
    				textInput.setBackground(Color.GRAY);
    				caret = textInput.getCaret();
    				caret.setSelectionVisible(false);
    				caret.setVisible(false);
    				textInput.setEditable(false);
    				//undone clear/change text of textinput?
				case KeyEvent.VK_F10:
					//e.consume();//todo necessary to consume?
					return true;
				}
				return false;
			}else{
				if(e.getID()==KeyEvent.KEY_PRESSED){
					switch (e.getKeyCode()) {
					case KeyEvent.VK_E:
						try {
							System.out.println("sleep started");
							Thread.sleep(6000);
							System.out.println("sleep ended");
						} catch (InterruptedException e1) {
							// TODO Auto-generated catch block
							e1.printStackTrace();
						}
						return true;
					case KeyEvent.VK_F3:
						//undone mark start searching
        				textInput.setBackground(Color.BLACK);
						break;
					case KeyEvent.VK_F4:
						//undone mark start binding
        				textInput.setBackground(Color.BLUE);
						break;
					case KeyEvent.VK_ESCAPE:
						//undone close modal group/dialog?
						return true;
					case KeyEvent.VK_SPACE:
						return true;
					default:
						return true;
					}
    				caret = textInput.getCaret();
    				caret.setSelectionVisible(true);
    				caret.setVisible(true);
    				textInput.setEditable(true);
				}
				return true;
			}
		}
    }
    
	
	private final KeyListener textInputKL = new KeyListener() {
		//note avoid alt key, it has special effect
		@Override
		public void keyPressed(KeyEvent e) {
			if(textInput.isEditable()){//undone, use '/' for search, 'r' for binding value, esc for cancellation, alt for confirmation!!!
				switch (e.getKeyCode()) {
				case KeyEvent.VK_ALT://undone
				case KeyEvent.VK_ESCAPE:
    				Caret caret = textInput.getCaret();
    				caret.setSelectionVisible(false);
    				caret.setVisible(false);
    				textInput.setEditable(false);
    				textInput.setBackground(Color.GRAY);
    				//undone things to process?
				}
			}else{
				Caret caret;
				switch (e.getKeyCode()) {
				//note the keys that can change caret location should never be used for editor actions, including: Home, End, Pageup, Pagedown, Left, Right, Up, Down!!
				//note as for the reason: imagine if the setSelectionVisible and setVisible don't work sometimes, then your caret can be very confusing for the user if it has any visual effect during the EditorPane Mode!
				case KeyEvent.VK_SLASH:
    				caret = textInput.getCaret();
    				caret.setSelectionVisible(true);
    				caret.setVisible(true);
    				textInput.setEditable(true);
    				textInput.setBackground(Color.BLACK);
					break;
				case KeyEvent.VK_R:
    				caret = textInput.getCaret();
    				caret.setSelectionVisible(true);
    				caret.setVisible(true);
    				textInput.setEditable(true);
    				textInput.setBackground(Color.BLUE);
    				
    				
    				break;
				case KeyEvent.VK_N:
					//undone search next occurrence of the string being searched for (in tab/modal dialog) 
					break;
				case KeyEvent.VK_H://? maybe I don't need h and l??
					//undone
					break;
				case KeyEvent.VK_L://? maybe I don't need h and l??
					//undone
					break;
				case KeyEvent.VK_K:
					for(int i=currentTab.selectedIndex-1; i>=0;i--){
						if(currentTab.components.get(i).isSelectable()){
							currentTab.selectedIndex = i;
							updateEditorText();
							break;
						}
					}
					break;
				case KeyEvent.VK_J:
					for(int i=currentTab.selectedIndex+1; i<currentTab.components.size();i++){
						if(currentTab.components.get(i).isSelectable()){
							currentTab.selectedIndex = i;
							updateEditorText();
							break;
						}
					}
					break;
    				//undone a lot of other cases
				case KeyEvent.VK_TAB:
					//undone use tab key to switch tabs
					break;
				}
			}
		}

		@Override
		public void keyReleased(KeyEvent e) {
		}

		@Override
		public void keyTyped(KeyEvent e) {
		}
	};

}
