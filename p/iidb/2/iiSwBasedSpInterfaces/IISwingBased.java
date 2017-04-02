package griddoor.iiSwBasedSpInterfaces;

import griddoor.SwingBasedEditorUI.EditorInterfaceJFrame;
import griddoor.util.UtilFuncs;

import java.io.File;
import java.lang.Thread.UncaughtExceptionHandler;
import java.util.Arrays;
import java.util.Calendar;
import java.util.GregorianCalendar;
import java.util.TimeZone;
import java.util.logging.Level;

public class IISwingBased {

	//undone input textarea can be used for 2 purposes: bind value & search the tab/modal dialog
	//undone for any string info displayed, you only have space that's less than one line, if it's not enough, then the string ends with ellipsis
	//undone you can see the complete info of any sense by selecting the part and press confirm to open a new tab about that sense
	
	//undone or maybe you should consider using modal dialog to do binding value. no, that's bad, you shouldn't use ANY swing features
	//undone remember swing is not good for cross-platform
	//undone also don't use window title for visual features



	public static void main(String[] args) {
//		GregorianCalendar gc = new GregorianCalendar(TimeZone.getTimeZone("UTC"));
//		gc.setTimeInMillis(1435740450000L);
//		System.out.println(gc.get(Calendar.HOUR_OF_DAY));
//		long ll = 257L;
//		byte bb = (byte) ll;
//		System.out.println(bb);
//		File f = new File("predef-0.2.tar.gz");
//		System.out.println(f.getParentFile().getName().length());
//		System.out.println(new File("tet.txt").delete());
//		System.out.println(UtilFuncs.convCommaSeparatedStrToBytea("234 ,54"));
//		System.out.println(UtilFuncs.convCommaSeparatedStrToBytea("234,54,"));
//		System.out.println(Arrays.toString(UtilFuncs.convCommaSeparatedStrToBytea("234,54")));
//		System.out.println(Arrays.toString(UtilFuncs.convCommaSeparatedStrToBytea("54")));
//		System.out.println(Arrays.toString(UtilFuncs.convCommaSeparatedStrToBytea("54")));
//		if(true)
//			return;
		//init modules
		


		//start UI
		//almost everything is done in event dispatch thread so there should be no visibility problem
		java.awt.EventQueue.invokeLater(new Runnable() {
			public void run() {

				for(String strv : args){
					int indofe = strv.indexOf('=');
					if(indofe==-1){
						switch(strv){
						//todo anything to add?
						default:
							System.out.println("Argument without value not recognized: "+strv);
							System.exit(0);
						}
					}else{
						String nm = strv.substring(0, indofe);
						switch(nm){
						//optional:
						case "hostNameOrAddr":
							Globals.hostNameOrAddr = strv.substring(indofe+1);
							break;
						//mandatory:
						case "defaultStorageLoca":
							Globals.defaultStorageLoca = strv.substring(indofe+1);
							break;
						case "defaultRegisteredLoca":
							Globals.defaultRegisteredLoca = strv.substring(indofe+1);
							break;
						case "defaultRegisteredLocSub":
							Globals.defaultRegisteredLocSub = strv.substring(indofe+1);
							break;
						case "exLogFullname":
							Globals.exLogFullname = strv.substring(indofe+1);
							break;
						default:
							System.out.println("Argument not recognized: "+strv);
							System.exit(0);
						}
					}
				}
				if(Globals.defaultStorageLoca==null) { System.out.println("defaultStorageLoca is absent."); System.exit(0); }
				if(Globals.defaultRegisteredLoca==null) { System.out.println("defaultRegisteredLoca is absent."); System.exit(0); }
				if(Globals.defaultRegisteredLocSub==null) { System.out.println("defaultRegisteredLocSub is absent."); System.exit(0); }
				if(Globals.exLogFullname==null) { System.out.println("exLogFullname is absent."); System.exit(0); }
				
				if(!java.awt.EventQueue.isDispatchThread())//debug
					System.out.println("NOT dispatching thread!!\n"+Thread.currentThread().getStackTrace()[0].toString());
				
				griddoor.SwingBasedEditorUI.Globals.loggerReq = new LazyLogger();
//				if(args.length==5)
//					Globals.hostNameOrAddr = args[4];
//				Globals.defaultStorageLoca = args[0];
//				Globals.defaultRegisteredLoca = args[1];
//				Globals.defaultRegisteredLocSub = args[2];
//				Globals.exLogFullname = args[3];
				Thread.currentThread().setUncaughtExceptionHandler(new UncaughtExceptionHandler() {
					@Override
					public void uncaughtException(Thread t, Throwable e) {
						Globals.getLogger().log(Level.SEVERE, e.getMessage(), e);
					}
				});
				Globals.mainJfr = new EditorInterfaceJFrame("II Swing-Based Specialized Interfaces");
				Globals.mainJfr.compileOp = " Storage, Reg, RegSub, ExLog: "+Globals.defaultStorageLoca+" "+Globals.defaultRegisteredLoca+" "+Globals.defaultRegisteredLocSub+" "+Globals.exLogFullname;
				Globals.mainJfr.addTabNSwitchToIt(new HomeTabPage(Globals.mainJfr.context));
				Globals.mainJfr.drawEditorText();
			}
		});
	}
}
