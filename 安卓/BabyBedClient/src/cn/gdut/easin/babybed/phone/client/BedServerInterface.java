package cn.gdut.easin.babybed.phone.client;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;
import java.net.UnknownHostException;

public class BedServerInterface {
	
	/*CONTROL COMMAND*/
	public static final int WARM_CTRL_OFF 		=	0x00000001;
	public static final int WARM_CTRL_ON		=  	0x00000002;
	public static final int PHOTO_CTR_ON 		= 	0x00000003;    //拍照
	public static final int CRADLE_CTRL_OFF  	=   0x00000004;    // 关闭摇床
	public static final int CRADLE_CTRL_ON  	=   0x00000005;    //打开摇床
	public static final int MUSIC_CTRL_NEXT  	=   0x00000006;    //控制下一曲
	public static final int MUSIC_CTRL_PRE 	=   0x00000007;   //控制上一曲
	public static final int MUSIC_CTRL_STOP 	=   0x00000008;   //控制暂停
	public static final int MUSIC_CTRL_ON   	=   0x00000009;     //控制播放
	public static final int GET_BED_DATA		=	0x0000000A;		 //请求获取婴儿床数据
	
	Socket socket = null;
	BufferedOutputStream bos = null;
	BufferedInputStream bis = null;
	
	private String mac = null;
	private String serverIp = "192.168.1.140";
	private int port = 8899; 
	
	
	public void setMac(String mac) {
		this.mac = mac;
	}
	
	public String getMac() {
		return this.mac;
	}
	
	public boolean SendDeviceMacType(String mac) throws IOException {
		byte[] macbytes = Packet.getDrivePacket(mac);
		bos.write(macbytes);
		bos.flush();		
		byte[] b = new byte[1024];
		int length = bis.read(b);
		if(length == -1) {
			return false;
		}
		return true;	
	}
	
	public boolean SendCommand(int cmd) throws IOException {
		byte[] bytes = Packet.getCommandPacket(cmd);
		bos.write(bytes);
		bos.flush();
		return true;

	}
	
	public BedStatus getBabyBedData() throws IOException{
		BedStatus status = new BedStatus();
		byte[] b = new byte[1024];
		int length = bis.read(b);
		if(length == -1) {
			return null;
		}		
		status = Packet.getBedStatus(b);		
		return status;
			
	}
	
	public BedStatus control(String ip, int port, String bedMac, int cmd) {
		BedStatus  status = null;
		this.serverIp = ip;
		this.port = port;
		this.mac = bedMac;
		try {
			socket = new Socket(serverIp, port);
			bos = new BufferedOutputStream(socket.getOutputStream());
			bis = new BufferedInputStream(socket.getInputStream());
			SendDeviceMacType(bedMac);
			SendCommand(cmd);
			status  = getBabyBedData();
			bos.close();
			bis.close();
			socket.close();
		} catch (UnknownHostException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}

		return status;
	}
	

}
