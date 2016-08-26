import cn.gdut.easin.babybed.phone.client.BedServerInterface;
import cn.gdut.easin.babybed.phone.client.BedStatus;
import cn.gdut.easin.babybed.phone.client.Packet;


public class test {
	public static void main(String[] arg) {

		BedServerInterface bs = new BedServerInterface();
		String mac = "98-59-ea-46-58-96";
		String ip = "192.168.1.124";
		int port = 9999;
		int cmd = BedServerInterface.GET_BED_DATA;
		
		BedStatus status = bs.control(ip, port, mac, cmd);//控制婴儿床,并获取婴儿床当前的状态
		
		System.out.println("婴儿床状态：");
		System.out.println("体温"+status.getBabyTemperature());
		System.out.println("室温温"+status.getRoomTemperature());
		String date = Packet.date2TimeStamp(status.getRecordTime(), "yyyy-MM-dd HH:mm:ss"); 
		System.out.println("日期"+date);
		System.out.println("哭"+status.isCry());
		System.out.println("加热"+status.isHeating());
		System.out.println("踢被子"+status.isKick());
		System.out.println("在床上"+status.isOnbed());
		System.out.println("尿"+status.isPee());
		System.out.println("播音乐"+status.isPlay());
		System.out.println("摇摆"+status.isSwing());
	}
}
