namespace WinHongKongArduino
{
    partial class WinHongKongArduino
    {
        /// <summary>
        /// 必要なデザイナー変数です。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 使用中のリソースをすべてクリーンアップします。
        /// </summary>
        /// <param name="disposing">マネージ リソースが破棄される場合 true、破棄されない場合は false です。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows フォーム デザイナーで生成されたコード

        /// <summary>
        /// デザイナー サポートに必要なメソッドです。このメソッドの内容を
        /// コード エディターで変更しないでください。
        /// </summary>
        private void InitializeComponent()
        {
            this.btnExtract = new System.Windows.Forms.Button();
            this.cmbCOMPort = new System.Windows.Forms.ComboBox();
            this.label1 = new System.Windows.Forms.Label();
            this.btnGetROMInfo = new System.Windows.Forms.Button();
            this.label2 = new System.Windows.Forms.Label();
            this.txtROMTitle = new System.Windows.Forms.TextBox();
            this.cmbROMType = new System.Windows.Forms.ComboBox();
            this.label3 = new System.Windows.Forms.Label();
            this.cmbROMCompo = new System.Windows.Forms.ComboBox();
            this.label4 = new System.Windows.Forms.Label();
            this.cmbROMSize = new System.Windows.Forms.ComboBox();
            this.label5 = new System.Windows.Forms.Label();
            this.cmbRAMSize = new System.Windows.Forms.ComboBox();
            this.label6 = new System.Windows.Forms.Label();
            this.statusStrip = new System.Windows.Forms.StatusStrip();
            this.toolStripProgressBar = new System.Windows.Forms.ToolStripProgressBar();
            this.toolStripStatusLabel = new System.Windows.Forms.ToolStripStatusLabel();
            this.statusStrip.SuspendLayout();
            this.SuspendLayout();
            // 
            // btnExtract
            // 
            this.btnExtract.Location = new System.Drawing.Point(168, 187);
            this.btnExtract.Name = "btnExtract";
            this.btnExtract.Size = new System.Drawing.Size(104, 23);
            this.btnExtract.TabIndex = 0;
            this.btnExtract.Text = "ROM吸い出し";
            this.btnExtract.UseVisualStyleBackColor = true;
            this.btnExtract.Click += new System.EventHandler(this.btnExtract_Click);
            // 
            // cmbCOMPort
            // 
            this.cmbCOMPort.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cmbCOMPort.FormattingEnabled = true;
            this.cmbCOMPort.Location = new System.Drawing.Point(84, 9);
            this.cmbCOMPort.Name = "cmbCOMPort";
            this.cmbCOMPort.Size = new System.Drawing.Size(76, 20);
            this.cmbCOMPort.TabIndex = 1;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(11, 125);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(61, 12);
            this.label1.TabIndex = 3;
            this.label1.Text = "ROMサイズ:";
            // 
            // btnGetROMInfo
            // 
            this.btnGetROMInfo.Location = new System.Drawing.Point(57, 187);
            this.btnGetROMInfo.Name = "btnGetROMInfo";
            this.btnGetROMInfo.Size = new System.Drawing.Size(104, 23);
            this.btnGetROMInfo.TabIndex = 4;
            this.btnGetROMInfo.Text = "ROM情報取得";
            this.btnGetROMInfo.UseVisualStyleBackColor = true;
            this.btnGetROMInfo.Click += new System.EventHandler(this.btnGetROMInfo_Click);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(11, 49);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(67, 12);
            this.label2.TabIndex = 5;
            this.label2.Text = "ROMタイトル:";
            // 
            // txtROMTitle
            // 
            this.txtROMTitle.Location = new System.Drawing.Point(84, 45);
            this.txtROMTitle.Name = "txtROMTitle";
            this.txtROMTitle.ReadOnly = true;
            this.txtROMTitle.Size = new System.Drawing.Size(163, 19);
            this.txtROMTitle.TabIndex = 6;
            // 
            // cmbROMType
            // 
            this.cmbROMType.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cmbROMType.FormattingEnabled = true;
            this.cmbROMType.Location = new System.Drawing.Point(84, 70);
            this.cmbROMType.Name = "cmbROMType";
            this.cmbROMType.Size = new System.Drawing.Size(163, 20);
            this.cmbROMType.TabIndex = 7;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(12, 73);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(50, 12);
            this.label3.TabIndex = 8;
            this.label3.Text = "マッピング:";
            // 
            // cmbROMCompo
            // 
            this.cmbROMCompo.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cmbROMCompo.FormattingEnabled = true;
            this.cmbROMCompo.Location = new System.Drawing.Point(84, 96);
            this.cmbROMCompo.Name = "cmbROMCompo";
            this.cmbROMCompo.Size = new System.Drawing.Size(163, 20);
            this.cmbROMCompo.TabIndex = 9;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(12, 99);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(56, 12);
            this.label4.TabIndex = 10;
            this.label4.Text = "ROM構成:";
            // 
            // cmbROMSize
            // 
            this.cmbROMSize.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cmbROMSize.FormattingEnabled = true;
            this.cmbROMSize.Location = new System.Drawing.Point(84, 122);
            this.cmbROMSize.Name = "cmbROMSize";
            this.cmbROMSize.Size = new System.Drawing.Size(163, 20);
            this.cmbROMSize.TabIndex = 11;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(11, 12);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(71, 12);
            this.label5.TabIndex = 12;
            this.label5.Text = "シリアルポート:";
            // 
            // cmbRAMSize
            // 
            this.cmbRAMSize.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cmbRAMSize.FormattingEnabled = true;
            this.cmbRAMSize.Location = new System.Drawing.Point(84, 148);
            this.cmbRAMSize.Name = "cmbRAMSize";
            this.cmbRAMSize.Size = new System.Drawing.Size(163, 20);
            this.cmbRAMSize.TabIndex = 14;
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(11, 151);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(61, 12);
            this.label6.TabIndex = 13;
            this.label6.Text = "RAMサイズ:";
            // 
            // statusStrip
            // 
            this.statusStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripProgressBar,
            this.toolStripStatusLabel});
            this.statusStrip.Location = new System.Drawing.Point(0, 230);
            this.statusStrip.Name = "statusStrip";
            this.statusStrip.Size = new System.Drawing.Size(284, 22);
            this.statusStrip.TabIndex = 15;
            // 
            // toolStripProgressBar
            // 
            this.toolStripProgressBar.Name = "toolStripProgressBar";
            this.toolStripProgressBar.Size = new System.Drawing.Size(100, 16);
            this.toolStripProgressBar.Visible = false;
            // 
            // toolStripStatusLabel
            // 
            this.toolStripStatusLabel.Name = "toolStripStatusLabel";
            this.toolStripStatusLabel.Size = new System.Drawing.Size(0, 17);
            // 
            // WinHongKongArduino
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(284, 252);
            this.Controls.Add(this.statusStrip);
            this.Controls.Add(this.cmbRAMSize);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.cmbROMSize);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.cmbROMCompo);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.cmbROMType);
            this.Controls.Add(this.txtROMTitle);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.btnGetROMInfo);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.cmbCOMPort);
            this.Controls.Add(this.btnExtract);
            this.Name = "WinHongKongArduino";
            this.Text = "WinHongKongArduino";
            this.Load += new System.EventHandler(this.WinHongKongArduino_Load);
            this.statusStrip.ResumeLayout(false);
            this.statusStrip.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button btnExtract;
        private System.Windows.Forms.ComboBox cmbCOMPort;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button btnGetROMInfo;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox txtROMTitle;
        private System.Windows.Forms.ComboBox cmbROMType;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.ComboBox cmbROMCompo;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.ComboBox cmbROMSize;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.ComboBox cmbRAMSize;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.StatusStrip statusStrip;
        private System.Windows.Forms.ToolStripProgressBar toolStripProgressBar;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabel;
    }
}

