#ifndef WidgetPlay_H
#define WidgetPlay_H

#include <QWidget>
#include <QLabel>
#include <QProgressBar>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMediaPlayer>
#include "PushButton.h"
#include "DoubanAPI.h"

class WidgetPlay : public QWidget
{
    Q_OBJECT
public:
    explicit WidgetPlay(QWidget *parent = 0);
	~WidgetPlay();
signals:

public slots:
	void userLogin(DoubanLogRetInfo loginInfo);
	void channelInfo(ChannelsInfo channelInfo);
	void songInfo(SongInfo songInfo);
private:
	QLabel *m_titleLab;			/**< 歌曲名字 */
	QLabel *m_artistLab;		/**< 演唱者 */
	QLabel *m_songCurTimeLab;	/**< 当前播放时间 */
	QLabel *m_songTotalTimeLab;	/**< 总共时间 */
	QProgressBar *m_playProPb;	/**< 播放进度 */
	PushButton *m_likeBtn;		/**< 喜欢 */
	PushButton *m_hateBtn;		/**< 讨厌按钮 */
	PushButton *m_pauseBtn;		/**< 暂停按钮 */
	PushButton *m_nextBtn;		/**< 下一曲 */

	DoubanAPI *m_doubanAPI;
	NetControl *m_nc;
	QMediaPlayer *m_play;
	Song m_song;
};

#endif // WidgetPlay_H
