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
		
		BedStatus status = bs.control(ip, port, mac, cmd);//����Ӥ����,����ȡӤ������ǰ��״̬
		
		System.out.println("Ӥ����״̬��");
		System.out.println("����"+status.getBabyTemperature());
		System.out.println("������"+status.getRoomTemperature());
		String date = Packet.date2TimeStamp(status.getRecordTime(), "yyyy-MM-dd HH:mm:ss"); 
		System.out.println("����"+date);
		System.out.println("��"+status.isCry());
		System.out.println("����"+status.isHeating());
		System.out.println("�߱���"+status.isKick());
		System.out.println("�ڴ���"+status.isOnbed());
		System.out.println("��"+status.isPee());
		System.out.println("������"+status.isPlay());
		System.out.println("ҡ��"+status.isSwing());
	}
}
