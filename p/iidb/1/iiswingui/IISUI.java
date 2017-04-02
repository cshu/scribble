package griddoor.iiswingui;

import griddoor.myanimelist.MALAnimeSearch;
import griddoor.util.*;

import java.awt.*;
import java.awt.event.*;

import javax.swing.*;
import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Date;
import java.util.concurrent.atomic.*;
import java.io.*;
import java.net.Socket;
import java.net.UnknownHostException;
import java.nio.charset.StandardCharsets;

//todo: add config file about exitAllowedWhenServerIsRunning

public class IISUI extends JFrame {
	public static final String myanimelistUsernamePasswd = "unm:passwd";
	public static final String registeredFolder = "pathofafolder";
	public static final String defaultStorageLoca = "pathoflocation";
	public static final int bytesLenUpperLimit = 50000;
	public static final String HOSTNAME = "127.0.0.1";
	public static final int PORT_NUMBER = 10004;
	public static final java.util.List<String> listOfClauseTypes = Arrays.<String>asList("indepprDUMMY","indepcl","connounvar","connounins","bytnounvar","bytnounins","dignounvar","dignounins","rawbytevar","rawbyteins","rawdigivar","rawdigiins","normalverb","normalnoun","adverbialcl","relativecla","gerundasobj","gerundassub","nounasobjec","nounassubje");
	public static boolean exitAllowedWhenServerIsRunning = false;
	public static boolean startServerAtStartUpWhenSerivceUnavail = true;

	public static Socket clientSocket;
	public static OutputStream outpStreSo;
	public static InputStream inpuStreSo;

	public abstract class IX {
		public byte[] a = new byte[8];
	}

	public class IB extends IX {
		public String b;
	}

	public class II extends IX {
		public byte[] b = new byte[8];
	}

	public class III extends II {
		public byte[] c = new byte[8];
	}

	public class IIII extends III {
		public byte[] d = new byte[8];
	}

	public class BI {
		public String a;
		public byte[] b = new byte[8];
	}

	public static final ArrayList<IB> invertiblev = new ArrayList<IB>();
	public static final ArrayList<IB> noninvertib = new ArrayList<IB>();
	public static final ArrayList<IB> bytnoun = new ArrayList<IB>();
	public static final ArrayList<IB> dignoun = new ArrayList<IB>();
	public static final ArrayList<IB> conroot = new ArrayList<IB>();
	public static final ArrayList<IB> absroot = new ArrayList<IB>();
	public static final ArrayList<II> mutexcl2 = new ArrayList<II>();
	public static final ArrayList<III> mutexcl3 = new ArrayList<III>();
	public static final ArrayList<II> subcate1 = new ArrayList<II>();
	public static final ArrayList<III> subcate2 = new ArrayList<III>();
	public static final ArrayList<III> inter2 = new ArrayList<III>();
	public static final ArrayList<III> union2 = new ArrayList<III>();
	public static final ArrayList<IIII> inter3 = new ArrayList<IIII>();
	public static final ArrayList<IIII> union3 = new ArrayList<IIII>();
	public static final ArrayList<III> relacomp2 = new ArrayList<III>();
	public static final ArrayList<IIII> relacomp3 = new ArrayList<IIII>();
	public static final ArrayList<BI> synonym = new ArrayList<BI>();
	public static final ArrayList<BI> inverseverb = new ArrayList<BI>();
	public static final ArrayList[] isnlALArr = { invertiblev, noninvertib, bytnoun, dignoun, conroot, absroot, mutexcl2, mutexcl3, subcate1, subcate2, inter2, union2, inter3, union3, relacomp2, relacomp3, synonym, inverseverb };

	public static final JEditorPane outJEP = new JEditorPane();
	public static final JTextArea textInput = new JTextArea();

	// todo: this variable is not fully utilized in program yet.
	public static final ArrayList<String> textInputCopies = new ArrayList<String>();

	public static IISUI theJfr;
	public static final SelableAppender sysOut = new SelableAppender();

	public IISUI(String e) {
		super(e);
		theJfr = this;

		this.setDefaultCloseOperation(JFrame.DO_NOTHING_ON_CLOSE);
		this.setExtendedState(JFrame.MAXIMIZED_BOTH);
		this.add(outJEP, BorderLayout.CENTER);
		this.add(textInput, BorderLayout.SOUTH);
		textInput.setFocusTraversalKeys(KeyboardFocusManager.FORWARD_TRAVERSAL_KEYS, null);
		textInput.setFocusTraversalKeys(KeyboardFocusManager.BACKWARD_TRAVERSAL_KEYS, null);
		outJEP.setEditable(false);
		// mainConsole.putClientProperty(JEditorPane.HONOR_DISPLAY_PROPERTIES,
		// Boolean.TRUE);
		outJEP.setFont(Font.getFont("BatangChe", new Font(Font.MONOSPACED, Font.PLAIN, 15)));
		textInput.setFont(Font.getFont("BatangChe", new Font(Font.MONOSPACED, Font.PLAIN, 15)));
		textInput.setBackground(Color.GRAY);
		textInput.setCaretColor(Color.WHITE);
		textInput.setForeground(Color.WHITE);
		outJEP.setBackground(Color.GRAY);
		outJEP.setCaretColor(Color.WHITE);
		outJEP.setForeground(Color.WHITE);
	}



	public static void LoadInfrastructureData() {
		invertiblev.clear();
		noninvertib.clear();
		bytnoun.clear();
		dignoun.clear();
		conroot.clear();
		absroot.clear();
		mutexcl2.clear();
		mutexcl3.clear();
		subcate1.clear();
		subcate2.clear();
		inter2.clear();
		union2.clear();
		inter3.clear();
		union3.clear();
		relacomp2.clear();
		relacomp3.clear();
		synonym.clear();
		inverseverb.clear();
		try {
			outpStreSo.write(0);
			byte[] buf = new byte[bytesLenUpperLimit];
			int isnlALArrInd = 0;
			for (ArrayList al : isnlALArr) {
				switch (isnlALArrInd) {
				case 0:
				case 1:
				case 2:
				case 3:
				case 4:
				case 5:
					while (inpuStreSo.read() == 0) {
						IB memb = theJfr.new IB();
						UtilFuncs.InputStreamReadBytesUntil(8, inpuStreSo, memb.a, 0);
						int lenOfBlob = inpuStreSo.read() + inpuStreSo.read() * 256;
						UtilFuncs.InputStreamReadBytesUntil(lenOfBlob, inpuStreSo, buf, 0);
						memb.b = new String(buf, 0, lenOfBlob, StandardCharsets.UTF_8);
						al.add(memb);
					}
					break;
				case 6:
				case 8:
					while (inpuStreSo.read() == 0) {
						II memb = theJfr.new II();
						UtilFuncs.InputStreamReadBytesUntil(8, inpuStreSo, memb.a, 0);
						UtilFuncs.InputStreamReadBytesUntil(8, inpuStreSo, memb.b, 0);
						al.add(memb);
					}
					break;
				case 7:
				case 9:
				case 10:
				case 11:
				case 14:
					while (inpuStreSo.read() == 0) {
						III memb = theJfr.new III();
						UtilFuncs.InputStreamReadBytesUntil(8, inpuStreSo, memb.a, 0);
						UtilFuncs.InputStreamReadBytesUntil(8, inpuStreSo, memb.b, 0);
						UtilFuncs.InputStreamReadBytesUntil(8, inpuStreSo, memb.c, 0);
						al.add(memb);
					}
					break;
				case 12:
				case 13:
				case 15:
					while (inpuStreSo.read() == 0) {
						IIII memb = theJfr.new IIII();
						UtilFuncs.InputStreamReadBytesUntil(8, inpuStreSo, memb.a, 0);
						UtilFuncs.InputStreamReadBytesUntil(8, inpuStreSo, memb.b, 0);
						UtilFuncs.InputStreamReadBytesUntil(8, inpuStreSo, memb.c, 0);
						UtilFuncs.InputStreamReadBytesUntil(8, inpuStreSo, memb.d, 0);
						al.add(memb);
					}
					break;
				case 16:
				case 17:
					while (inpuStreSo.read() == 0) {
						BI memb = theJfr.new BI();
						int lenOfBlob = inpuStreSo.read() + inpuStreSo.read() * 256;
						UtilFuncs.InputStreamReadBytesUntil(lenOfBlob, inpuStreSo, buf, 0);
						memb.a = new String(buf, 0, lenOfBlob, StandardCharsets.UTF_8);
						UtilFuncs.InputStreamReadBytesUntil(8, inpuStreSo, memb.b, 0);
						al.add(memb);
					}
					break;
				}
				isnlALArrInd++;
			}
			if (inpuStreSo.available() > 0)
				sysOut.atAlias.append("\nGet ISNL sense not over exception!");
			// todo: if still available, throw exception.
		} catch (IOException e) {
			e.printStackTrace();
			sysOut.atAlias.append("\n" + e.toString());
		}

	}

	private static final DocumentListener textInputDL = new DocumentListener(){

		@Override
		public void insertUpdate(DocumentEvent e) {
			IISUI.tc.sels.get(IISUI.tc.ind).FuncInsOrRemUpdate();
		}

		@Override
		public void removeUpdate(DocumentEvent e) {
			IISUI.tc.sels.get(IISUI.tc.ind).FuncInsOrRemUpdate();
		}

		@Override
		public void changedUpdate(DocumentEvent e) {
		}
		
	};

	private static final KeyListener textInputKL = new KeyListener() {
		@Override
		public void keyPressed(KeyEvent e) {
			System.out.println("textInpPr");
		}

		@Override
		public void keyReleased(KeyEvent e) {
			System.out.println("textInpRe");
			switch (e.getKeyCode()) {
			case KeyEvent.VK_ESCAPE:
				tc.sels.get(tc.ind).GetStrToFillJta();
			}
		}

		@Override
		public void keyTyped(KeyEvent e) {
			System.out.println("textInpTy");
		}
	};

	private static final KeyListener jepKeyListener = new KeyListener() {
		private int pressedKeyCode;

		@Override
		public void keyPressed(KeyEvent e) {
			System.out.println("jepPr");
			pressedKeyCode = e.getKeyCode();
			tc.sels.get(tc.ind).FuncKeyPressed(e);
		}

		@Override
		public void keyReleased(KeyEvent e) {
			System.out.println("jepRe");
			if (e.getKeyCode() != pressedKeyCode)
				return;
			tc.FuncKeyRelease(e);
		}

		@Override
		public void keyTyped(KeyEvent e) {
			System.out.println("jepTy");
		}
	};



	private static final FocusListener jepFocusListener = new FocusListener() {
		@Override
		public void focusGained(FocusEvent e) {
			System.out.println("jepFocusGain");
			// todo: when window lost focus, freeze everything
			// todo: after window gain focus, nothing can be changed
			// todo: you have to press tab several time to recover to the last
			// state
			if(!isFocusInJtaElseJep)
				return;
			isFocusInJtaElseJep = false;
			textInput.getDocument().removeDocumentListener(textInputDL);
			if (textInput.getText().getBytes(StandardCharsets.UTF_8).length <= bytesLenUpperLimit) {
				if (textInput.getText().length() > 0) {
					if (textInputCopies.size() == 0 || !textInputCopies.get(textInputCopies.size() - 1).equals(textInput.getText()))
						textInputCopies.add(textInput.getText());
					tc.sels.get(tc.ind).FuncStrTraverseFromJta(textInput.getText());
					textInput.setText("");
				}
			} else {
				// todo: warn user?
			}
		}

		@Override
		public void focusLost(FocusEvent e) {
			System.out.println("jepFocusLost");
		}
	};
	
	public static boolean isFocusInJtaElseJep = false;
	
	private static final FocusListener textInputFL = new FocusListener() {
		@Override
		public void focusGained(FocusEvent e) {
			System.out.println("textInputFocusGain");
			textInput.setBackground(Color.BLACK);
			if (isFocusInJtaElseJep)
				return;
			isFocusInJtaElseJep = true;
			tc.sels.get(tc.ind).GetStrToFillJta();
			textInput.getDocument().addDocumentListener(textInputDL);
			
		}

		@Override
		public void focusLost(FocusEvent e) {
			System.out.println("textInputFocusLost");
			textInput.setBackground(Color.GRAY);
		}
	};


	public static final String keyObjH = "H";
	public static final String keyObjL = "L";
	public static final String keyObjK = "K";
	public static final String keyObjJ = "J";
	public static final InputMap iMap = outJEP.getInputMap(JComponent.WHEN_FOCUSED);
	public static final ActionMap aMap = outJEP.getActionMap();

	private static final long serialVersionUID = 1L;

	public static final TextConsoleBasic tcBas = new TextConsoleBasic();
	public static final TextConsoleClause tcCla = new TextConsoleClause();
	public static final TextConsoleSense tcSen = new TextConsoleSense();
	public static final TextConsolePredicates tcPre = new TextConsolePredicates();
	public static final TextConsoleFiles tcFil = new TextConsoleFiles();
	public static final TextConsoleMAL tcMal = new TextConsoleMAL();
	public static TextConsole tc;
	public static final Selectable newlineSel = new Selectable("\n ");

	public static void main(String[] args) {
		MALAnimeSearch.usernamePasswd = myanimelistUsernamePasswd;
		
		java.awt.EventQueue.invokeLater(new Runnable() {
			public void run() {
				new IISUI("II Management System Swing UI").setVisible(true);
				outJEP.requestFocusInWindow();
				tc = tcBas;

				tcBas.sels.add(Utils.MacrRefrSelAs2ndLast(new QueIisuiInitial()));
				tcCla.sels.add(new Selectable("CLAUSE MANAGEMENT MODE"));
				tcSen.sels.add(new Selectable("SENSE MANAGEMENT MODE"));
				tcPre.sels.add(new Selectable(tcPre.title));
				tcFil.sels.add(new Selectable(tcFil.title));
				tcMal.sels.add(new Selectable(tcMal.title));
				
				tcBas.sels.add(newlineSel);
				tcCla.sels.add(newlineSel);
				tcSen.sels.add(newlineSel);
				tcPre.sels.add(newlineSel);
				tcFil.sels.add(newlineSel);
				tcMal.sels.add(newlineSel);

				tcCla.sels.addAll(Arrays.asList(Utils.MacrRefrSelAs2ndLast(new ClauSlot()),newlineSel));
				tcSen.sels.addAll(Arrays.asList(Utils.MacrRefrSelAs2ndLast(new SensSlot()),Utils.MacrRefrSelAs2ndLast(new SensCategory()),newlineSel));

				tcBas.sels.add(null);
				tcCla.sels.add(null);
				tcSen.sels.add(null);
				tcPre.sels.add(null);
				tcFil.sels.add(null);
				tcMal.sels.add(null);
				sysOut.atAlias.append("\n\nSYSOUT");
				tcBas.sels.add(sysOut);
				tcCla.sels.add(sysOut);
				tcSen.sels.add(sysOut);
				tcPre.sels.add(sysOut);
				tcFil.sels.add(sysOut);
				tcMal.sels.add(sysOut);
				tcBas.Set2ndLastSel();
				tcCla.Set2ndLastSel();
				tcSen.Set2ndLastSel();
				tcPre.Set2ndLastSel();
				tcFil.Set2ndLastSel();
				tcMal.Set2ndLastSel();
				tcBas.CalcSelectionPos();
				tcCla.CalcSelectionPos();
				tcSen.CalcSelectionPos();
				tcPre.CalcSelectionPos();
				tcFil.CalcSelectionPos();
				tcMal.CalcSelectionPos();

				tc.DrawTextAndSelection();

				iMap.put(KeyStroke.getKeyStroke(KeyEvent.VK_H, 0), keyObjH);
				aMap.put(keyObjH, new AbstractAction() {
					@Override
					public void actionPerformed(ActionEvent e) {
						if (tc.ind != 0) {
							// remove
							// setCurrentParagAndReCalcSelectionPosAndSet2ndLastParag(currTC.ind
							// - 1);
							tc.ind--;
							
							tc.sels.get(tc.ind).FuncFocusGained();
							tc.Set2ndLastSel();
							tc.CalcSelectionPos();
							tc.DrawTextAndSelection();
						}
					}
				});
				iMap.put(KeyStroke.getKeyStroke(KeyEvent.VK_L, 0), keyObjL);
				aMap.put(keyObjL, new AbstractAction() {
					@Override
					public void actionPerformed(ActionEvent e) {
						if (tc.ind + 1 < tc.sels.size()) {
							// remove
							// setCurrentParagAndReCalcSelectionPosAndSet2ndLastParag(currTC.ind
							// + 1);
							tc.ind++;

							tc.sels.get(tc.ind).FuncFocusGained();
							tc.Set2ndLastSel();
							tc.CalcSelectionPos();
							tc.DrawTextAndSelection();
						}
					}
				});
				iMap.put(KeyStroke.getKeyStroke(KeyEvent.VK_K, 0), keyObjK);
				aMap.put(keyObjK, new AbstractAction() {
					@Override
					public void actionPerformed(ActionEvent e) {
						if (tc.ind == 0)
							return;
						int cur = 0;
						while (tc.sels.get(tc.ind - cur) != newlineSel) {
							cur++;
							if (tc.ind == cur)
								return;
						}
						int abov = 1;
						while (tc.sels.get(tc.ind - cur - abov) != newlineSel) {
							if (tc.ind == cur + abov)
								break;
							abov++;
						}
						if (abov > cur)
							tc.ind -= abov;
						else
							tc.ind -= cur;

						tc.sels.get(tc.ind).FuncFocusGained();
						tc.Set2ndLastSel();
						tc.CalcSelectionPos();
						tc.DrawTextAndSelection();
					}
				});
				iMap.put(KeyStroke.getKeyStroke(KeyEvent.VK_TAB, 0), keyObjJ);
				aMap.put(keyObjJ, new AbstractAction() {
					@Override
					public void actionPerformed(ActionEvent e) {
						System.out.println("jepAction");
						int cur = 0;
						while (tc.sels.get(tc.ind - cur) != newlineSel && tc.ind != cur)
							cur++;
						for (int i = 1;; i++) {
							if (tc.ind + i == tc.sels.size())
								return;
							if (tc.sels.get(tc.ind + i) == newlineSel) {
								tc.ind += i;
								break;
							}
						}
						do {
							if (tc.ind + 1 == tc.sels.size() || cur == 0)
								break;
							tc.ind++;
							cur--;
						} while (tc.sels.get(tc.ind) != newlineSel);

						tc.sels.get(tc.ind).FuncFocusGained();
						tc.Set2ndLastSel();
						tc.CalcSelectionPos();
						tc.DrawTextAndSelection();
					}
				});


				tiMap.put(KeyStroke.getKeyStroke(KeyEvent.VK_TAB, 0), "d");
				taMap.put("d", new AbstractAction() {
					@Override
					public void actionPerformed(ActionEvent e) {
						System.out.println("textInpAction");
					}
				});
				outJEP.addKeyListener(jepKeyListener);
				outJEP.addFocusListener(jepFocusListener);
				textInput.addFocusListener(textInputFL);
				textInput.addKeyListener(textInputKL);
				theJfr.addKeyListener(testKeyListener);
				
				KeyboardFocusManager manager = KeyboardFocusManager.getCurrentKeyboardFocusManager();
				manager.addKeyEventDispatcher (theJfr.new MyDispatcher());
			}
		});
	}
    class MyDispatcher implements KeyEventDispatcher {
        @Override
        public boolean dispatchKeyEvent(KeyEvent e) {
            if (e.getID() == KeyEvent.KEY_PRESSED) {
                System.out.println("1test1");
                //System.out.println(e.getKeyCode());
            } else if (e.getID() == KeyEvent.KEY_RELEASED) {
                System.out.println("2test2");
            } else if (e.getID() == KeyEvent.KEY_TYPED) {
                System.out.println("3test3");
            } else
            	System.out.println(e.getID());
            return false;
        }
    }
	public static final InputMap tiMap = textInput.getInputMap(JComponent.WHEN_FOCUSED);
	public static final ActionMap taMap = textInput.getActionMap();

	private static final KeyListener testKeyListener = new KeyListener() {

		@Override
		public void keyPressed(KeyEvent e) {
			System.out.println("testPr");
		}

		@Override
		public void keyReleased(KeyEvent e) {
			System.out.println("testRe");
		}

		@Override
		public void keyTyped(KeyEvent e) {
			System.out.println("testTy");
		}
	};
}
