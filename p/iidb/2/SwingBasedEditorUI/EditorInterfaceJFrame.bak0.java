package griddoor.SwingBasedEditorUI;

import griddoor.logging.StaticLogger;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Font;
import java.awt.HeadlessException;
import java.awt.KeyEventDispatcher;
import java.awt.KeyboardFocusManager;
import java.awt.event.FocusEvent;
import java.awt.event.FocusListener;
import java.awt.event.KeyEvent;

import javax.swing.JEditorPane;
import javax.swing.JFrame;
import javax.swing.JTextArea;

public class EditorInterfaceJFrame extends JFrame {

	private static final long serialVersionUID = 1L;

	public static final JEditorPane outJEP = new JEditorPane();
	public static final JTextArea textInput = new JTextArea();
	public EditorInterfaceJFrame(String title) throws HeadlessException {
		super(title);
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);//undone remove
		//setDefaultCloseOperation(JFrame.DO_NOTHING_ON_CLOSE);
		setExtendedState(JFrame.MAXIMIZED_BOTH);
		add(outJEP, BorderLayout.CENTER);
		add(textInput, BorderLayout.SOUTH);
		
		//? you cannot remove traversal because if you remove these 2 lines then tab key will be typed into the TextArea as text???
		//textInput.setFocusTraversalKeys(KeyboardFocusManager.FORWARD_TRAVERSAL_KEYS, null);
		//textInput.setFocusTraversalKeys(KeyboardFocusManager.BACKWARD_TRAVERSAL_KEYS, null);
		
		outJEP.setFocusable(false);
		outJEP.setEditable(false);
		outJEP.setFont(new Font("Consolas", Font.PLAIN, 15));
		textInput.setFont(new Font("Consolas", Font.PLAIN, 15));
		outJEP.setBackground(Color.GRAY);
		outJEP.setCaretColor(Color.WHITE);
		outJEP.setForeground(Color.WHITE);
		textInput.setBackground(Color.GRAY);
		textInput.setCaretColor(Color.WHITE);
		textInput.setForeground(Color.WHITE);

		setVisible(true);
		outJEP.requestFocusInWindow();//todo why can't we have default focus?
		
		//undone
		Globals.getLogger().severe("test");
		

		//KeyboardFocusManager manager = KeyboardFocusManager.getCurrentKeyboardFocusManager();
		//manager.addKeyEventDispatcher (this.new GlobalKeyEventDispatcher());
	}

	//note why use key_pressed: release behavior differs on different OSs, so just use press and so the user won't have any reason to keep pressing a key, which fires events over and over.
    private class GlobalKeyEventDispatcher implements KeyEventDispatcher {
    	private boolean focusOwnerIsTextInputElseOutJep;
    	
    	//note I test many times and confirmed that TRAVERSAL depends on KEY_PRESSED, not release or type. And typing in TextArea does NOT depend on KEY_PRESSED!
    	
        @Override
        public boolean dispatchKeyEvent(KeyEvent e) {
        	if(e.getID()==KeyEvent.KEY_PRESSED){
        		//todo performance hit here??
            	if(outJEP.isFocusOwner() == focusOwnerIsTextInputElseOutJep){//note this case may be due to switching applications, so no action is taken except traverse
            		Globals.getLogger().warning("debug is this commonly reachable?");//debug remove
            		if(focusOwnerIsTextInputElseOutJep)
            			textInput.requestFocusInWindow();
            		else
            			outJEP.requestFocusInWindow();
            		return true;//undone problem unsolved! when you switch from another app to jump into textarea, it still types one character before it restores to normal!
            		//undone let's use another approach: only textarea is focusable! use tab to change from editing and not editing
            		//undone or use approach 2: use F1 key to do traversing!
            		
            		
            	}
        		if(e.getKeyCode()==KeyEvent.VK_TAB){
        			if(focusOwnerIsTextInputElseOutJep){
        				textInput.setBackground(Color.GRAY);
        				focusOwnerIsTextInputElseOutJep = false;
        			}else{
        				textInput.setBackground(Color.BLACK);
        				focusOwnerIsTextInputElseOutJep = true;
        			}
//        			if(outJEP.isFocusOwner()){
//        				if(!textInput.requestFocusInWindow()){
//        					StaticLogger.Log("textinput trying requestfocusinwindow, but failed");
//        					return true;
//        				}
//        				focusOwnerIsTextInputElseOutJep = true;
//        			}else{
//        				if(!outJEP.requestFocusInWindow()){
//        					StaticLogger.Log("outjep trying requestfocusinwindow, but failed");
//        					return true;
//        				}
//        				focusOwnerIsTextInputElseOutJep = false;
//        			}
        			
        			
        			//undone some operations after tab key?
        			return false;//note return false to trigger traversal
        		}else if(focusOwnerIsTextInputElseOutJep){
        			return false;//note you have to return false here, or you cannot use LEFT and RIGHT keys, event Ctrl+C doesn't work
        		}else{
                	//undone all keypress are handled here
        			//undone switch a lot?
        			return true;
        		}
        	}else
        		return false;//note you have to return false or you cannot type in textarea
        }
    }
}
