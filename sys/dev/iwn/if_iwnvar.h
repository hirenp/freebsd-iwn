/*	$FreeBSD: head/sys/dev/iwn/if_iwnvar.h 221651 2011-05-08 12:06:12Z bschmidt $	*/
/*	$OpenBSD: if_iwnvar.h,v 1.18 2010/04/30 16:06:46 damien Exp $	*/

/*-
 * Copyright (c) 2007, 2008
 *	Damien Bergamini <damien.bergamini@free.fr>
 * Copyright (c) 2008 Sam Leffler, Errno Consulting
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#define IWN_MAX_BLINK_TBL       10
#define IWN_LED_STATIC_ON       0
#define IWN_LED_STATIC_OFF      1
#define IWN_LED_SLOW_BLINK      2
#define IWN_LED_INT_BLINK       3
#define IWN_LED_UNIT            0x1388 /* 5 ms */

static const struct {
        uint16_t tpt;   /* Mb/s */
        uint8_t on_time;
        uint8_t off_time;
} blink_tbl[] =
{
        {300, 5,  5},
        {200, 8,  8},
        {100, 11, 11},
        {70,  13, 13},
        {50,  15, 15},
        {20,  17, 17},
        {10,  19, 19},
        {5,   22, 22},
        {1,   26, 26},
        {0,   33, 33},
        /* SOLID_ON */
};

struct iwn_led_mode
{
        uint8_t         led_cur_mode;
        uint64_t        led_cur_bt;
        uint64_t        led_last_bt;
        uint64_t        led_cur_tpt;
        uint64_t        led_last_tpt;
        uint64_t        led_bt_diff;
        int             led_cur_time;
        int             led_last_time;
};

struct iwn_rx_radiotap_header {
	struct ieee80211_radiotap_header wr_ihdr;
	uint64_t	wr_tsft;
	uint8_t		wr_flags;
	uint8_t		wr_rate;
	uint16_t	wr_chan_freq;
	uint16_t	wr_chan_flags;
	int8_t		wr_dbm_antsignal;
	int8_t		wr_dbm_antnoise;
} __packed;

#define IWN_RX_RADIOTAP_PRESENT						\
	((1 << IEEE80211_RADIOTAP_TSFT) |				\
	 (1 << IEEE80211_RADIOTAP_FLAGS) |				\
	 (1 << IEEE80211_RADIOTAP_RATE) |				\
	 (1 << IEEE80211_RADIOTAP_CHANNEL) |				\
	 (1 << IEEE80211_RADIOTAP_DBM_ANTSIGNAL) |			\
	 (1 << IEEE80211_RADIOTAP_DBM_ANTNOISE))

struct iwn_tx_radiotap_header {
	struct ieee80211_radiotap_header wt_ihdr;
	uint8_t		wt_flags;
	uint8_t		wt_rate;
	uint16_t	wt_chan_freq;
	uint16_t	wt_chan_flags;
} __packed;

#define IWN_TX_RADIOTAP_PRESENT						\
	((1 << IEEE80211_RADIOTAP_FLAGS) |				\
	 (1 << IEEE80211_RADIOTAP_RATE) |				\
	 (1 << IEEE80211_RADIOTAP_CHANNEL))

struct iwn_dma_info {
	bus_dma_tag_t		tag;
	bus_dmamap_t		map;
	bus_dma_segment_t	seg;
	bus_addr_t		paddr;
	caddr_t			vaddr;
	bus_size_t		size;
};

struct iwn_tx_data {
	bus_dmamap_t		map;
	bus_addr_t		cmd_paddr;
	bus_addr_t		scratch_paddr;
	struct mbuf		*m;
	struct ieee80211_node	*ni;
};

struct iwn_tx_ring {
	struct iwn_dma_info	desc_dma;
	struct iwn_dma_info	cmd_dma;
	struct iwn_tx_desc	*desc;
	struct iwn_tx_cmd	*cmd;
	struct iwn_tx_data	data[IWN_TX_RING_COUNT];
	bus_dma_tag_t		data_dmat;
	int			qid;
	int			queued;
	int			cur;
	int			read;
};

struct iwn_softc;

struct iwn_rx_data {
	struct mbuf	*m;
	bus_dmamap_t	map;
};

struct iwn_rx_ring {
	struct iwn_dma_info	desc_dma;
	struct iwn_dma_info	stat_dma;
	uint32_t		*desc;
	struct iwn_rx_status	*stat;
	struct iwn_rx_data	data[IWN_RX_RING_COUNT];
	bus_dma_tag_t		data_dmat;
	int			cur;
};

struct iwn_node {
	struct	ieee80211_node		ni;	/* must be the first */
	uint16_t			disable_tid;
	uint8_t				id;
	uint32_t			ridx[256];
	struct {
		uint64_t		bitmap;
		int			startidx;
		int			nframes;
	} agg[IEEE80211_TID_SIZE];
};

struct iwn_calib_state {
	uint8_t		state;
#define IWN_CALIB_STATE_INIT	0
#define IWN_CALIB_STATE_ASSOC	1
#define IWN_CALIB_STATE_RUN	2

	u_int		nbeacons;
	uint32_t	noise[3];
	uint32_t	rssi[3];
	uint32_t	ofdm_x1;
	uint32_t	ofdm_mrc_x1;
	uint32_t	ofdm_x4;
	uint32_t	ofdm_mrc_x4;
	uint32_t	cck_x4;
	uint32_t	cck_mrc_x4;
	uint32_t	bad_plcp_ofdm;
	uint32_t	fa_ofdm;
	uint32_t	bad_plcp_cck;
	uint32_t	fa_cck;
	uint32_t	low_fa;
	uint8_t		cck_state;
#define IWN_CCK_STATE_INIT	0
#define IWN_CCK_STATE_LOFA	1
#define IWN_CCK_STATE_HIFA	2

	uint8_t		noise_samples[20];
	u_int		cur_noise_sample;
	uint8_t		noise_ref;
	uint32_t	energy_samples[10];
	u_int		cur_energy_sample;
	uint32_t	energy_cck;
};

struct iwn_calib_info {
	uint8_t		*buf;
	u_int		len;
};

struct iwn_fw_part {
	const uint8_t	*text;
	uint32_t	textsz;
	const uint8_t	*data;
	uint32_t	datasz;
};

struct iwn_fw_info {
	const uint8_t		*data;
	size_t			size;
	struct iwn_fw_part	init;
	struct iwn_fw_part	main;
	struct iwn_fw_part	boot;
};

struct iwn_ops {
	int		(*load_firmware)(struct iwn_softc *);
	void		(*read_eeprom)(struct iwn_softc *);
	int		(*post_alive)(struct iwn_softc *);
	int		(*nic_config)(struct iwn_softc *);
	void		(*update_sched)(struct iwn_softc *, int, int, uint8_t,
			    uint16_t);
	int		(*get_temperature)(struct iwn_softc *);
	int		(*get_rssi)(struct iwn_softc *, struct iwn_rx_stat *);
	int		(*set_txpower)(struct iwn_softc *,
			    struct ieee80211_channel *, int);
	int		(*init_gains)(struct iwn_softc *);
	int		(*set_gains)(struct iwn_softc *);
	int		(*add_node)(struct iwn_softc *, struct iwn_node_info *,
			    int);
	void		(*tx_done)(struct iwn_softc *, struct iwn_rx_desc *,
			    struct iwn_rx_data *);
	void		(*ampdu_tx_start)(struct iwn_softc *,
			    struct ieee80211_node *, int, uint8_t, uint16_t);
	void		(*ampdu_tx_stop)(struct iwn_softc *, int, uint8_t,
			    uint16_t);
};

struct iwn_vap {
	struct ieee80211vap	iv_vap;
	uint8_t			iv_ridx;

	int			(*iv_newstate)(struct ieee80211vap *,
				    enum ieee80211_state, int);
        int                     ctx;
        int                     beacon_int;
        uint8_t                 macaddr[IEEE80211_ADDR_LEN];
};
#define	IWN_VAP(_vap)	((struct iwn_vap *)(_vap))

enum iwn_rxon_ctx_id {
        IWN_RXON_BSS_CTX,
        IWN_RXON_PAN_CTX,
        IWN_NUM_RXON_CTX
};

#define IWN_UC_PAN_PRESENT              1

/* ADD / MODIFY STATION Command (Op Code 18) -  byte 76-18 -bit13
        STA_FLAG_PAN_STATION bit:
        This bit is set (1) for a station in PAN mode */
#define STA_FLAG_PAN_STATION            (1 << 13)

#define BEACON_INTERVAL_DEFAULT         200
#define IWN_SLOT_TIME_MIN               20

struct iwn_pan_slot {
        uint16_t time;
        uint8_t type;
        uint8_t reserved;
} __packed;

struct iwn_pan_params_cmd {
        uint16_t flags;
#define IWN_PAN_PARAMS_FLG_SLOTTED_MODE (1 << 3)

        uint8_t reserved;
        uint8_t num_slots;
        struct iwn_pan_slot slots[10];
} __packed;

#define LINK_QUAL_AC_NUM 4

struct iwn_link_qual_general_params {
        uint8_t flags;

        /* No entries at or above this (driver chosen) index contain MIMO */
        uint8_t mimo_delimiter;

        /* Best single antenna to use for single stream (legacy, SISO). */
        uint8_t single_stream_ant_msk;  /* LINK_QUAL_ANT_* */

        /* Best antennas to use for MIMO (unused for 4965, assumes both). */
        uint8_t dual_stream_ant_msk;    /* LINK_QUAL_ANT_* */

        /*
         * If driver needs to use different initial rates for different
         * EDCA QOS access categories (as implemented by tx fifos 0-3),
         * this table will set that up, by indicating the indexes in the
         * rs_table[LINK_QUAL_MAX_RETRY_NUM] rate table at which to start.
         * Otherwise, driver should set all entries to 0.
         *
         * Entry usage:
         * 0 = Background, 1 = Best Effort (normal), 2 = Video, 3 = Voice
         * TX FIFOs above 3 use same value (typically 0) as TX FIFO 3.
         */
        uint8_t start_rate_index[LINK_QUAL_AC_NUM];
} __packed;

#define LINK_QUAL_MAX_RETRY_NUM 16

struct iwn_softc {
	device_t		sc_dev;

	struct ifnet		*sc_ifp;
	int			sc_debug;

	struct mtx		sc_mtx;

	u_int			sc_flags;
#define IWN_FLAG_HAS_OTPROM	(1 << 1)
#define IWN_FLAG_CALIB_DONE	(1 << 2)
#define IWN_FLAG_USE_ICT	(1 << 3)
#define IWN_FLAG_INTERNAL_PA	(1 << 4)
#define IWN_FLAG_HAS_11N	(1 << 6)
#define IWN_FLAG_ENH_SENS	(1 << 7)
#define IWN_FLAG_ADV_BTCOEX	(1 << 8)

	uint8_t 		hw_type;

	struct iwn_ops		ops;
	const char		*fwname;
	const struct iwn_sensitivity_limits
				*limits;
	int			ntxqs;
	int			firstaggqueue;
	int			ndmachnls;
	uint8_t			broadcast_id;
	int			rxonsz;
	int			schedsz;
	uint32_t		fw_text_maxsz;
	uint32_t		fw_data_maxsz;
	uint32_t		fwsz;
	bus_size_t		sched_txfact_addr;
	uint32_t		reset_noise_gain;
	uint32_t		noise_gain;

	/* TX scheduler rings. */
	struct iwn_dma_info	sched_dma;
	uint16_t		*sched;
	uint32_t		sched_base;

	/* "Keep Warm" page. */
	struct iwn_dma_info	kw_dma;

	/* Firmware image. */
	const struct firmware	*fw_fp;

	/* Firmware DMA transfer. */
	struct iwn_dma_info	fw_dma;

	/* ICT table. */
	struct iwn_dma_info	ict_dma;
	uint32_t		*ict;
	int			ict_cur;

	/* TX/RX rings. */
	struct iwn_tx_ring	txq[IWN5000_NTXQUEUES];
	struct iwn_rx_ring	rxq;

	int			mem_rid;
	struct resource		*mem;
	bus_space_tag_t		sc_st;
	bus_space_handle_t	sc_sh;
	int			irq_rid;
	struct resource		*irq;
	void 			*sc_ih;
	bus_size_t		sc_sz;
	int			sc_cap_off;	/* PCIe Capabilities. */

	/* Tasks used by the driver */
	struct task		sc_reinit_task;
	struct task		sc_radioon_task;
	struct task		sc_radiooff_task;

	struct callout		calib_to;
	int			calib_cnt;
	struct iwn_calib_state	calib;
	struct callout		watchdog_to;
	struct callout		ct_kill_exit_to;

	struct iwn_fw_info	fw;
	struct iwn_calib_info	calibcmd[5];
	uint32_t		errptr;

	struct iwn_rx_stat	last_rx_stat;
	int			last_rx_valid;
	struct iwn_ucode_info	ucode_info;
	struct iwn_rxon         rx_on[IWN_NUM_RXON_CTX];
	struct iwn_rxon		*rxon;
	int                     ctx;
        struct ieee80211vap     *ivap[IWN_NUM_RXON_CTX];
	uint8_t                 uc_pan_support;
        uint8_t                 uc_scan_progress;

	uint32_t		rawtemp;
	int			temp;
	int			noise;
	uint32_t		qfullmsk;

	uint32_t		prom_base;
	struct iwn4965_eeprom_band
				bands[IWN_NBANDS];
	struct iwn_eeprom_chan	eeprom_channels[IWN_NBANDS][IWN_MAX_CHAN_PER_BAND];
	uint16_t		rfcfg;
	uint8_t			calib_ver;
	char			eeprom_domain[4];
	uint32_t		eeprom_crystal;
	int16_t			eeprom_temp;
	int16_t			eeprom_voltage;
	int8_t			maxpwr2GHz;
	int8_t			maxpwr5GHz;
	int8_t			maxpwr[IEEE80211_CHAN_MAX];

	uint32_t                tlv_feature_flags;

	int32_t			temp_off;
	uint32_t		int_mask;
	uint8_t			ntxchains;
	uint8_t			nrxchains;
	uint8_t			txchainmask;
	uint8_t			rxchainmask;
	uint8_t			chainmask;

	int			sc_tx_timer;
	int                     sc_scan_timer;

	struct ieee80211_tx_ampdu *qid2tap[IWN5000_NTXQUEUES];

	int			(*sc_ampdu_rx_start)(struct ieee80211_node *,
				    struct ieee80211_rx_ampdu *, int, int, int);
	void			(*sc_ampdu_rx_stop)(struct ieee80211_node *,
				    struct ieee80211_rx_ampdu *);
	int			(*sc_addba_request)(struct ieee80211_node *,
				    struct ieee80211_tx_ampdu *, int, int, int);
	int			(*sc_addba_response)(struct ieee80211_node *,
				    struct ieee80211_tx_ampdu *, int, int, int);
	void			(*sc_addba_stop)(struct ieee80211_node *,
				    struct ieee80211_tx_ampdu *);

	struct iwn_led_mode 	sc_led;

	struct iwn_rx_radiotap_header sc_rxtap;
	struct iwn_tx_radiotap_header sc_txtap;

        /* The power save level originally configured by user */
        int                     desired_pwrsave_level;

        /* The current power save level, this may differ from the configured value due to
         * thermal throttling etc.
         * */
        int                     current_pwrsave_level;

};


#define IWN_LOCK_INIT(_sc) \
	mtx_init(&(_sc)->sc_mtx, device_get_nameunit((_sc)->sc_dev), \
	    MTX_NETWORK_LOCK, MTX_DEF)
#define IWN_LOCK(_sc)			mtx_lock(&(_sc)->sc_mtx)
#define IWN_LOCK_ASSERT(_sc)		mtx_assert(&(_sc)->sc_mtx, MA_OWNED)
#define IWN_UNLOCK(_sc)			mtx_unlock(&(_sc)->sc_mtx)
#define IWN_LOCK_DESTROY(_sc)		mtx_destroy(&(_sc)->sc_mtx)

#define IWN_POWERSAVE_LVL_NONE                  0
#define IWN_POWERSAVE_LVL_VOIP_COMPATIBLE       1
#define IWN_POWERSAVE_LVL_MAX                   5

/*
 * By default we enable power saving. An alternative is to by default use best
 * speed/latency mode (no power saving), which would be in line with the
 * previous driver implementation. The default level can be changed easily from
 * the line below.
 *
 * Note that once the interface wlan0 etc. is created, the user/system can
 * change the mode using ifconfig/ioctl.  
 */
#define IWN_POWERSAVE_LVL_DEFAULT       IWN_POWERSAVE_LVL_VOIP_COMPATIBLE

/* DTIM value to pass in for IWN_POWERSAVE_LVL_VOIP_COMPATIBLE */
#define IWN_POWERSAVE_DTIM_VOIP_COMPATIBLE 2

/*
 * If IWL_DTIM_INDICATES_UNICAST_PENDING_AT_AP is defined, then power saving
 * (including the power saving done for unicast traffic) becomes * proportional
 * to the DTIM period received from the AP. Otherwise the constant DTIM
 * period IWL_POWERSAVE_DTIM_VOIP_COMPATIBLE is used.
 *
 * Per the 802.11 spec DTIM value as applicable to power saving seems to
 * be relevant only for indicating the frequency at which broadcast/multicast
 * data is sent to the PS STAs.
 * However in practice some APs may also send the unicast traffic along with
 * the DTIM.
 */
#define IWN_DTIM_INDICATES_UNICAST_PENDING_AT_AP

const uint8_t iwn_bss_ac_to_queue[] = {
        2, 3, 1, 0,
};

const uint8_t iwn_pan_ac_to_queue[] = {
        5, 4, 6, 7,
};

