package griddoor.iiSwBasedSpInterfaces;

import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.GregorianCalendar;
import java.util.TimeZone;
import java.util.logging.Level;

import griddoor.IIPropSets.Formatters;
import griddoor.IIPropSets.MalAnime;
import griddoor.IIPropSets.SingleInstanceSense;
import griddoor.SwingBasedEditorUI.ButtonWithFixedText;
import griddoor.SwingBasedEditorUI.ButtonWithVal;
import griddoor.SwingBasedEditorUI.CheckBoxWithVal;
import griddoor.SwingBasedEditorUI.ComponentGroup;
import griddoor.SwingBasedEditorUI.JavaStringTextArea;
import griddoor.SwingBasedEditorUI.NewTabPageLink;
import griddoor.SwingBasedEditorUI.TextComponent;
import griddoor.SwingBasedEditorUI.TextTabPage;
import griddoor.SwingBasedEditorUI.UIContext;
import griddoor.util.FortyBitSecTime;
import griddoor.util.UtilFuncs;

public class FileSystemTabPage extends TextTabPage {

	public FileSystemTabPage(UIContext context) {
		super(context);
		
		components.add(new JavaStringTextArea(this, ""){
			@Override
			public void fillByPrepending() {
				fillEditorWithLines(tyname+"Explore a location:"+javaStr+"\n");
			}
			@Override
			public void bindStrFunc() {
				javaStr = cg.context.editor.textInput.getText();
				File fileObj = UtilFuncs.getFileFromReadablePath(javaStr);
				if (!fileObj.isDirectory()){
					showMsgBox("Not a directory.");
					this.cg.context.redrawNeeded = true;
					return;
				}
				File[] items = fileObj.listFiles();
				if(items.length==0){
					showMsgBox("Empty directory, not supported.");
					this.cg.context.redrawNeeded = true;
					return;
				}
				for(File item : items)
					if(!item.isFile()){
						showMsgBox("Some are not files, not supported.");
						this.cg.context.redrawNeeded = true;
						return;
					}
				this.cg.modalD = new ComponentGroup(this.cg);
				for(File item : items){
					String str = Formatters.format(item).append('\n').toString();
					this.cg.modalD.components.add(new CheckBoxWithVal<File>(this.cg.modalD,item,str));
				}
				//undone SHOW TOTAL NUMBER OF FILES!!!!
				this.cg.modalD.components.add(new ButtonWithFixedText(this.cg.modalD, "Invert Selection"){
					@Override
					public void spacePressed() {
						for(int i = this.cg.cursorIndex-1;i>=0;i--){
							CheckBoxWithVal<?> cb = (CheckBoxWithVal<?>)this.cg.components.get(i);
							cb.checked = !cb.checked;
						}
						this.cg.context.redrawNeeded = true;
					}
				});
				this.cg.modalD.components.add(new ButtonWithFixedText(this.cg.modalD, "Register And Copy As ..."){
					@Override
					public void spacePressed() {
						if(MalAnime.candidates.size()==0){
							showMsgBox("You don't have candidate senses for file registration.");
							this.cg.context.redrawNeeded = true;
							return;
						}
						this.cg.modalD = new ComponentGroup(this.cg);
						for(MalAnime ma : MalAnime.candidates){
							this.cg.modalD.components.add(new ButtonWithVal<MalAnime>(this.cg.modalD,ma){
								@Override
								public void xPressed(){
									MalAnime.candidates.remove(this.val);
									this.val.chkcr = 254;
									if(this.cg.components.size()==1){
										this.cg.closeItself();
									}else{
										this.cg.components.remove(this.cg.cursorIndex);
										if(this.cg.components.size()==this.cg.cursorIndex)
											this.cg.cursorIndex--;
									}
									this.cg.context.redrawNeeded = true;
								}
								@Override
								public void spacePressed() {
									ArrayList<File> arroff = new ArrayList<>();
									for(int i = this.cg.cg.cursorIndex-2;i>=0;i--){
										CheckBoxWithVal<?> cb = (CheckBoxWithVal<?>)this.cg.cg.components.get(i);
										if(!((File)cb.val).exists()){
											showMsgBox("Some file(s) don't exist any more, need to reload");
											this.cg.context.redrawNeeded = true;
											return;
										}
									}
									for(int i = this.cg.cg.cursorIndex-2;i>=0;i--){
										CheckBoxWithVal<?> cb = (CheckBoxWithVal<?>)this.cg.cg.components.get(i);
										if(cb.checked){
											arroff.add((File)cb.val);
											this.cg.cg.components.remove(i);
										}
									}
									GregorianCalendar gc = new GregorianCalendar(TimeZone.getTimeZone("UTC"));
									for(File fil : arroff){
										try {
											Globals.getOuStre().write(Globals.INSERT_FILEALLOCNAME);
											long size = fil.length();
											UtilFuncs.outputStreamSendAsUnsigned(Globals.getOuStre(), size);
											long wtime = fil.lastModified();
											gc.setTimeInMillis(wtime);
											UtilFuncs.outputStreamSendLen24AndBytes(Globals.getOuStre(),FortyBitSecTime.convertFromCalendar(gc));
											String onm = fil.getName();
											UtilFuncs.outputStreamSendLen24AndStrUtf8(Globals.getOuStre(), onm);
											SingleInstanceSense filSense = SingleInstanceSense.recvFileJustAlloc(onm, size, wtime);
											char driveCh = fil.getCanonicalPath().charAt(0);
											if(Character.toLowerCase(driveCh)==Character.toLowerCase(Globals.defaultStorageLoca.charAt(0))){
												if(!fil.renameTo(new File(Globals.defaultStorageLoca+filSense.gets(Globals.FILE_snm)))){
													//undone better handling required!!
													Globals.getLogger().severe("Failed to move "+onm+" TO:"+Globals.defaultStorageLoca+filSense.gets(Globals.FILE_snm));
													showMsgBox("Failed to move "+onm);
													this.cg.context.redrawNeeded = true;
													return;
												}
											}else{
												Files.copy(fil.toPath(), Paths.get(Globals.defaultStorageLoca+filSense.gets(Globals.FILE_snm)));
												String parentnm = fil.getParentFile().getName();
												if(parentnm.isEmpty())
													parentnm = Globals.defaultRegisteredLocSub;
												File dirFileObj = new File(driveCh+Globals.defaultRegisteredLoca + parentnm);
												if(!dirFileObj.exists())
													dirFileObj.mkdir();//fixme what if it's a file, not a dir?? although file is forbidden under regFolder in the first place
												File newFileObj = new File(driveCh+Globals.defaultRegisteredLoca + parentnm + File.separatorChar + onm);
												for(int i=0;newFileObj.exists();i++)
													newFileObj = new File(driveCh+Globals.defaultRegisteredLoca + parentnm + File.separatorChar + onm + i);
												fil.renameTo(newFileObj);//undone if return false, show warning message after all copying is done.
											}
											Globals.getOuStre().write(Globals.INSERT_DATASTOREINFO);
											UtilFuncs.outputStreamSendInt64(Globals.getOuStre(), filSense.id);
											UtilFuncs.outputStreamSendInt64(Globals.getOuStre(), this.val.sensid);
											if(Globals.getInStre().read()!=0){
												Globals.getLogger().severe("Failed to insert record about data storing info: "+onm);
												showMsgBox("Failed to insert record about data storing info: "+onm);
												this.cg.context.redrawNeeded = true;
												return;
											}
										} catch (IOException e) {
											//undone better handling required!!
											Globals.getLogger().log(Level.SEVERE, e.getMessage(), e);
											showMsgBox("Exception thrown when copying file: "+fil.getName());
											this.cg.context.redrawNeeded = true;
											return;
										}
									}
									this.cg.cg.cursorIndex = 0;
									this.cg.closeItself();
									this.cg.context.redrawNeeded = true;
								};
							});
						}
						this.cg.context.redrawNeeded = true;
					}
				});
				this.cg.context.redrawNeeded = true;
			}
		});
	}
	
	
	public static NewTabPageLink instantiateNewTabPageLink(ComponentGroup cgOfLink){
		return new NewTabPageLink(cgOfLink){
			@Override
			public NewTabPageLink cloneCom(ComponentGroup newcg) {
				return instantiateNewTabPageLink(newcg);
			}
			@Override
			public void fillByPrepending() {
				fillEditorWithLines(tyname+"File system page in new tab\n");
			}
			@Override
			public void spacePressed() {
				this.cg.context.editor.addTabNSwitchToIt(new FileSystemTabPage(this.cg.context));
			}
		};
	}
	
	@Override
	public String toString() {
		return "File Systems";
	}

}
