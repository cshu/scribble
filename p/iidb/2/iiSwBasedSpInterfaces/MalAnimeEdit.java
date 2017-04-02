package griddoor.iiSwBasedSpInterfaces;

import java.io.File;
import java.io.IOException;
import java.net.UnknownHostException;
import java.text.DecimalFormat;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.Iterator;
import java.util.logging.Level;

import griddoor.IIPropSets.Formatters;
import griddoor.IIPropSets.SingleInstanceSense;
import griddoor.SwingBasedEditorUI.ButtonWithFixedText;
import griddoor.SwingBasedEditorUI.ButtonWithStriVal;
import griddoor.SwingBasedEditorUI.ButtonWithVal;
import griddoor.SwingBasedEditorUI.ComponentGroupCanBeClosedWithEsc;
import griddoor.SwingBasedEditorUI.JavaStringTextArea;
import griddoor.SwingBasedEditorUI.TextTabPage;
import griddoor.SwingBasedEditorUI.UIContext;
import griddoor.util.TupleOfTwoObj;
import griddoor.util.UtilFuncs;

public class MalAnimeEdit extends TextTabPage {

	public MalAnimeEdit(UIContext context, long id) throws UnknownHostException, IOException {
		super(context);
		Globals.getOuStre().write(Globals.SELECT_ANIME);
		UtilFuncs.outputStreamSendInt64(Globals.getOuStre(), id);
		SingleInstanceSense malanime = SingleInstanceSense.recvMalAnimeInstance(id);
		byte[] prog = UtilFuncs.inputStreamRecvBytea(Globals.getInStre());
		byte[] rank = UtilFuncs.inputStreamRecvBytea(Globals.getInStre());
		String comm = UtilFuncs.inputStreamRecvUtf8String(Globals.getInStre());
		ArrayList<SingleInstanceSense> files = new ArrayList<>();
		while(true){
			SingleInstanceSense fil = SingleInstanceSense.recvFileInstance();
			if(fil==null)
				break;
			files.add(fil);
		}
		components.add(new ButtonWithStriVal(this,malanime.gets(Globals.ANIMATION_mid)));//todo make this a HYPERLINK?
		//components.add(new JStrTextAreaWithLabelNSynopsis(this,"Title",malanime.gets(Globals.ANIMATION_title)){
		//	@Override
		//	public void bindStrFunc() {
		//		super.bindStrFunc();//todo too dangerous? add confirmation?
		//		//undone send to server?
		//	}
		//});
		components.add(new ButtonWithStriVal(this,"Title: "+malanime.gets(Globals.ANIMATION_title)));
		components.add(new ButtonWithStriVal(this,"English: "+malanime.gets(Globals.ANIMATION_english)));
		components.add(new ButtonWithStriVal(this,"Synonyms: "+malanime.gets(Globals.ANIMATION_synonyms)));
		components.add(new ButtonWithStriVal(this,"Episodes: "+malanime.gets(Globals.ANIMATION_episodes)));
		components.add(new ButtonWithStriVal(this,"Type: "+malanime.gets(Globals.ANIMATION_type)));
		for(int i=Globals.ANIMATION_ess;i<malanime.strs.size();i++)
			components.add(new ButtonWithStriVal(this,"Essential Text: "+malanime.gets(i)));//todo make editable & addition-able?
		Globals.getOuStre().write(Globals.SELECT_SAPROP);
		UtilFuncs.outputStreamSendInt64(Globals.getOuStre(), id);
		StringBuilder verbsb = new StringBuilder("Edit props: ");
		do{
			SingleInstanceSense vsens = SingleInstanceSense.recvVerb();
			if(vsens == null)
				break;
			verbsb.append(vsens.gets(0)).append('`');
		}while(true);
		components.add(new ButtonWithStriVal(this,verbsb.toString()){
			@Override
			public void spacePressed() {
				try {
					this.cg.context.editor.addTabNSwitchToIt(new SingleArgPropsEdit(this.cg.context,id,"[MalAnime] "+UtilFuncs.getFirstLine(malanime.gets(Globals.ANIMATION_title))));
				} catch (IOException e) {
					Globals.getLogger().log(Level.SEVERE, e.getMessage(), e);
					showMsgBox("Exception thrown when trying to load edit page");
					this.cg.context.redrawNeeded = true;
				}
			}
		});
		JavaStringTextArea progTe = new JavaStringTextArea(this, UtilFuncs.convByteaToCommaSeparatedStr(prog)){
			@Override
			public void fillByPrepending() {
				fillEditorWithLines("PROG: "+this.javaStr+"\n");
			}
		};
		components.add(progTe);
		JavaStringTextArea rankTe = new JavaStringTextArea(this, UtilFuncs.convByteaToCommaSeparatedStr(rank)){
			@Override
			public void fillByPrepending() {
				fillEditorWithLines("RANK: "+this.javaStr+"\n");
			}
		};
		components.add(rankTe);
		JavaStringTextArea commTe = new JavaStringTextArea(this, comm){
			@Override
			public void fillByPrepending() {
				fillEditorWithLines("COMM: "+this.javaStr+"\n");
			}
		};
		components.add(commTe);
		for(SingleInstanceSense fil : files){
			final String lineToFillWith = Formatters.formatAsFileStorage(fil).append('\n').toString();
			components.add(new ButtonWithVal<TupleOfTwoObj<SingleInstanceSense,ArrayList<SingleInstanceSense>>>(this,new TupleOfTwoObj<SingleInstanceSense,ArrayList<SingleInstanceSense>>(fil,files)){
				@Override
				public void fillByPrepending() {
					fillEditorWithLines(lineToFillWith);
				};
				@Override
				public void xPressed() {
					ComponentGroupCanBeClosedWithEsc modalDToUse = new ComponentGroupCanBeClosedWithEsc(this.cg);
					modalDToUse.components.add(new ButtonWithFixedText(modalDToUse,"Delete this ONE file about animation"){
						@Override
						public void spacePressed() {
							SingleInstanceSense sis = val.t1;
							if(new File(Globals.defaultStorageLoca+sis.gets(Globals.FILE_snm)).delete()){
								try {
									int err = sis.heuristicallyDeleteFromFileStorageNDataStInfo();
									if(err!=0){
										showMsgBox("Deletion in storage device succeeded, but error code has been returned from server: "+err);
										this.cg.context.redrawNeeded = true;
										return;
									}
								} catch (IOException e) {
									Globals.getLogger().log(Level.SEVERE, e.getMessage(), e);
									showMsgBox("Deletion in storage device succeeded, but exception has been thrown when deleting from DB");
									this.cg.context.redrawNeeded = true;
									return;
								}
								val.t2.remove(sis);
								this.cg.cg.components.remove(this.cg.cg.cursorIndex);
								this.cg.closeItself();
								this.cg.context.redrawNeeded = true;
							}else{
								this.showMsgBox("Deletion failed");
								this.cg.context.redrawNeeded = true;
								return;
							}
						}
					});
					modalDToUse.components.add(new ButtonWithFixedText(modalDToUse,"Delete ALL files below (including this one) about animation"){
						@Override
						public void spacePressed() {
							boolean startedRemoving = false;
							for(Iterator<SingleInstanceSense> iter = val.t2.iterator(); iter.hasNext();){
								SingleInstanceSense sis = iter.next();
								if(!startedRemoving){
									if(sis==val.t1)
										startedRemoving = true;
									else
										continue;
								}
								if(new File(Globals.defaultStorageLoca+sis.gets(Globals.FILE_snm)).delete()){
									try {
										int err = sis.heuristicallyDeleteFromFileStorageNDataStInfo();
										if(err!=0){
											showMsgBox("Deletion in storage device succeeded, but error code has been returned from server: "+err);
											this.cg.context.redrawNeeded = true;
											return;
										}
									} catch (IOException e) {
										Globals.getLogger().log(Level.SEVERE, e.getMessage(), e);
										showMsgBox("Deletion in storage device succeeded, but exception has been thrown when deleting from DB");
										this.cg.context.redrawNeeded = true;
										return;
									}
									iter.remove();
									this.cg.cg.components.remove(this.cg.cg.cursorIndex);
								}else{
									this.showMsgBox("Deletion failed");
									this.cg.context.redrawNeeded = true;
									return;
								}
							}
							this.cg.closeItself();
							this.cg.context.redrawNeeded = true;
						}
					});
					this.cg.modalD = modalDToUse;
					this.cg.context.redrawNeeded = true;
				};
			});
		}
		components.add(new ButtonWithFixedText(this,"SAVE"){
			@Override
			public void spacePressed() {
				try {
					if(progTe.javaStr.length()==0&&rankTe.javaStr.length()==0&&commTe.javaStr.length()==0){
						Globals.getOuStre().write(Globals.DELETE_ANIMATION);
						UtilFuncs.outputStreamSendInt64(Globals.getOuStre(), id);
						if(Globals.getInStre().read()==0){
							showMsgBox("Deleted from sp_animationp");
						}else{
							Globals.getLogger().severe("Failed to upsert animation:"+id);
							showMsgBox("Failed to delete from sp_animationp");
						}
					}else{
						byte[] progbuf, rankbuf;
						if((progbuf = UtilFuncs.convCommaSeparatedStrToBytea(progTe.javaStr))==null||(rankbuf = UtilFuncs.convCommaSeparatedStrToBytea(rankTe.javaStr))==null){
							showMsgBox("Progress/Rank not formatted correctly");
							this.cg.context.redrawNeeded = true;
							return;
						}
						Globals.getOuStre().write(Globals.UPSERT_ANIMATION);
						UtilFuncs.outputStreamSendInt64(Globals.getOuStre(), id);
						UtilFuncs.outputStreamSendLen24AndBytes(Globals.getOuStre(),progbuf);
						UtilFuncs.outputStreamSendLen24AndBytes(Globals.getOuStre(),rankbuf);
						UtilFuncs.outputStreamSendLen24AndStrUtf8(Globals.getOuStre(),commTe.javaStr);
						if(Globals.getInStre().read()==0){
							showMsgBox("Saved successfully");
						}else{
							Globals.getLogger().severe("Failed to upsert animation:"+id);
							showMsgBox("Failed to upsert animation");
						}
					}
				} catch (IOException e) {
					Globals.getLogger().log(Level.SEVERE, e.getMessage(), e);
					showMsgBox("Exception thrown when upserting/deleting animation!");
				}
				this.cg.context.redrawNeeded = true;
				return;
			}
		});
	}

	@Override
	public String toString() {
		return "Animation Page";
	}
}
