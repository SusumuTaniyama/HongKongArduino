using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace WinHongKongArduino
{
    public partial class WinHongKongArduino : Form
    {
        public WinHongKongArduino()
        {
            InitializeComponent();
        }

        private void WinHongKongArduino_Load(object sender, EventArgs e)
        {
            string[] portNames = SerialPort.GetPortNames();

            foreach (string portName in portNames)
            {
                cmbCOMPort.Items.Add(portName);
            }

            cmbCOMPort.SelectedIndex = cmbCOMPort.Items.Count - 1;

            // マッピング
            cmbROMType.Items.Add("LoROM");
            cmbROMType.Items.Add("HiROM");

            // ROM構成
            cmbROMCompo.Items.Add("ROM");
            cmbROMCompo.Items.Add("ROM+RAM");
            cmbROMCompo.Items.Add("ROM+SRAM");

            // ROMサイズ
            cmbROMSize.Items.Add("8MB");
            cmbROMSize.Items.Add("4MB");
            cmbROMSize.Items.Add("2MB");
            cmbROMSize.Items.Add("1MB");
            cmbROMSize.Items.Add("512KB");
            cmbROMSize.Items.Add("256KB");

            // RAMサイズ
            cmbRAMSize.Items.Add("なし");
            cmbRAMSize.Items.Add("2KB");
            cmbRAMSize.Items.Add("4KB");
            cmbRAMSize.Items.Add("8KB");
            cmbRAMSize.Items.Add("16KB");
            cmbRAMSize.Items.Add("32KB");
            cmbRAMSize.Items.Add("64KB");
            cmbRAMSize.Items.Add("128KB");
        }

        // 吸い出し
        private void btnExtract_Click(object sender, EventArgs e)
        {
            // 入力チェック
            if (cmbROMType.SelectedItem == null)
            {
                MessageBox.Show("マッピングが選択されていません。");
                return;
            }
            if (cmbROMSize.SelectedItem == null)
            {
                MessageBox.Show("ROMサイズが選択されていません。");
                return;
            }

            // ファイルダイアログ
            SaveFileDialog sfd = new SaveFileDialog();
            sfd.Filter = "SFC ROMファイル(*.sfc)|*.sfc|すべてのファイル(*.*)|*.*";
            if (sfd.ShowDialog() != DialogResult.OK)
            {
                return;
            }

            // ROMサイズ取得
            int romsize = 0;
            switch (cmbROMSize.SelectedItem.ToString())
            {
                case "8MB":
                    romsize = 0x800000;
                    break;
                case "4MB":
                    romsize = 0x400000;
                    break;
                case "2MB":
                    romsize = 0x200000;
                    break;
                case "1MB":
                    romsize = 0x100000;
                    break;
                case "512KB":
                    romsize = 0x80000;
                    break;
                case "256KB":
                    romsize = 0x40000;
                    break;
            }

            byte[] data = new byte[romsize];

            bool isLoROM = (cmbROMType.SelectedItem.ToString() == "LoROM");

            // プログレスバー表示
            toolStripProgressBar.Visible = true;
            toolStripProgressBar.Maximum = romsize;

            // ステータス表示
            toolStripStatusLabel.Text = "吸い出し中...";

            // ボタン無効
            btnGetROMInfo.Enabled = false;
            btnExtract.Enabled = false;

            // 吸い出し
            sendControl(12); // OE + CS + !WE + !RST
            readROM(data, 0, 0, romsize, isLoROM);

            // ファイル書き込み
            File.WriteAllBytes(sfd.FileName, data);

            // ステータス表示
            toolStripStatusLabel.Text = "吸い出し完了";

            // ボタン有効
            btnGetROMInfo.Enabled = true;
            btnExtract.Enabled = true;
        }

        // ROM情報取得
        private void btnGetROMInfo_Click(object sender, EventArgs e)
        {
            byte[] data = new byte[25];
            sendControl(12); // OE + CS + !WE + !RST
            readROM(data, 0, 0xFFC0, data.Length);

            // ROMタイトル
            string title = "";
            for (int i = 0; i < 21; i++)
            {
                title += Convert.ToChar(data[i]);
            }
            txtROMTitle.Text = title;

            // マッピング
            if ((data[21] & 0x01) == 0)
            {
                // LoROM
                cmbROMType.SelectedItem = "LoROM";
            }
            else
            {
                // HiROM
                cmbROMType.SelectedItem = "HiROM";
            }

            // ROM構成
            switch (data[22])
            {
                case 0x00:
                    cmbROMCompo.SelectedItem = "ROM";
                    break;
                case 0x01:
                    cmbROMCompo.SelectedItem = "ROM+RAM";
                    break;
                case 0x02:
                    cmbROMCompo.SelectedItem = "ROM+SRAM";
                    break;
            }

            // ROMサイズ
            switch (data[23])
            {
                case 0x0D:
                    cmbROMSize.SelectedItem = "8MB";
                    break;
                case 0x0C:
                    cmbROMSize.SelectedItem = "4MB";
                    break;
                case 0x0B:
                    cmbROMSize.SelectedItem = "2MB";
                    break;
                case 0x0A:
                    cmbROMSize.SelectedItem = "1MB";
                    break;
                case 0x09:
                    cmbROMSize.SelectedItem = "512KB";
                    break;
                case 0x08:
                    cmbROMSize.SelectedItem = "256KB";
                    break;
            }

            // RAMサイズ
            switch (data[24])
            {
                case 0x00:
                    cmbRAMSize.SelectedItem = "なし";
                    break;
                case 0x01:
                    cmbRAMSize.SelectedItem = "2KB";
                    break;
                case 0x02:
                    cmbRAMSize.SelectedItem = "4KB";
                    break;
                case 0x03:
                    cmbRAMSize.SelectedItem = "8KB";
                    break;
                case 0x04:
                    cmbRAMSize.SelectedItem = "16KB";
                    break;
                case 0x05:
                    cmbRAMSize.SelectedItem = "32KB";
                    break;
                case 0x06:
                    cmbRAMSize.SelectedItem = "64KB";
                    break;
                case 0x07:
                    cmbRAMSize.SelectedItem = "128KB";
                    break;
            }
        }

        private void readROM(byte[] data, int offset, int address, int size, bool isLoROM = false)
        {
            string portName = cmbCOMPort.SelectedItem.ToString();
            SerialPort port = new SerialPort(portName, 115200);

            port.Open();

            byte[] cmd = new byte[7];
            if (isLoROM)
            {
                cmd[0] = (byte)'r'; // LoROM
            }
            else
            {
                cmd[0] = (byte)'R'; // HiROM
            }

            cmd[1] = (byte)(address >> (8 * 0));
            cmd[2] = (byte)(address >> (8 * 1));
            cmd[3] = (byte)(address >> (8 * 2));

            cmd[4] = (byte)(size >> (8 * 0));
            cmd[5] = (byte)(size >> (8 * 1));
            cmd[6] = (byte)(size >> (8 * 2));

            port.Write(cmd, 0, cmd.Length);

            int readbyte = 0;
            while (readbyte < size)
            {
                readbyte += port.Read(data, offset + readbyte, size - readbyte);

                // 進行状況表示
                toolStripProgressBar.Value = readbyte;
                Application.DoEvents();

                System.Threading.Thread.Sleep(1);
            }

            port.Close();
            port.Dispose();
        }

        private void sendControl(byte b)
        {
            string portName = cmbCOMPort.SelectedItem.ToString();
            SerialPort port = new SerialPort(portName, 115200);

            port.Open();

            byte[] cmd = new byte[2];
            cmd[0] = (byte)'c';
            cmd[1] = b;
            port.Write(cmd, 0, cmd.Length);

            port.Close();
            port.Dispose();
        }
    }
}
