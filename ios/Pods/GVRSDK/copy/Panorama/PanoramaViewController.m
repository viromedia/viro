#import "PanoramaViewController.h"

#import "GVRPanoramaView.h"

static const CGFloat kMargin = 16;
static const CGFloat kPanoViewHeight = 250;

@interface PanoramaViewController ()<GVRWidgetViewDelegate>

@end

@implementation PanoramaViewController {
  GVRPanoramaView *_panoView;
  UIScrollView *_scrollView;
  UILabel *_titleLabel;
  UILabel *_subtitleLabel;
  UILabel *_preambleLabel;
  UILabel *_captionLabel;
  UILabel *_postambleLabel;
  UITextView *_attributionTextView;
}

- (void)viewDidLoad {
  [super viewDidLoad];

  self.title = @"Panorama";
  self.view.backgroundColor = [UIColor whiteColor];

  _scrollView = [[UIScrollView alloc] initWithFrame:self.view.bounds];
  _scrollView.autoresizingMask = UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight;
  [self.view addSubview:_scrollView];

  _titleLabel =
      [self createLabelWithFontSize:20 bold:YES text:@"Machu Picchu\nWorld Heritage Site"];
  [_scrollView addSubview:_titleLabel];

  _subtitleLabel = [self createLabelWithFontSize:14 text:@"The world-famous citadel of the Andes"];
  _subtitleLabel.textColor = [UIColor darkGrayColor];
  [_scrollView addSubview:_subtitleLabel];

  _preambleLabel =
      [self createLabelWithFontSize:16
                               text:@"Machu Picchu is an Incan citadel set high in the Andes "
                               @"Mountains in Peru, above the Urubamba River valley."];
  [_scrollView addSubview:_preambleLabel];

  _panoView = [[GVRPanoramaView alloc] init];
  _panoView.delegate = self;
  _panoView.enableFullscreenButton = YES;
  _panoView.enableCardboardButton = YES;
  _panoView.enableTouchTracking = YES;
  [_panoView loadImage:[UIImage imageNamed:@"andes.jpg"]
                ofType:kGVRPanoramaImageTypeStereoOverUnder];
  [_scrollView addSubview:_panoView];

  _captionLabel = [self createLabelWithFontSize:14 text:@"A 360 panoramic view of Machu Picchu"];
  _captionLabel.textColor = [UIColor darkGrayColor];
  [_scrollView addSubview:_captionLabel];

  _postambleLabel = [self
      createLabelWithFontSize:16
                         text:@"It is situated on a mountain ridge above the Sacred Valley which "
                         @"is 80 kilometres (50 mi) northwest of Cuzco and through which the "
                         @"Urubamba River flows. Most archaeologists believe that Machu "
                         @"Picchu was built as an estate for the Inca emperor Pachacuti "
                         @"(1438–1472). Often mistakenly referred to as the \"Lost City of "
                         @"the Incas\" (a title more accurately applied to Vilcabamba), it "
                         @"is the most familiar icon of Inca civilization.\n\nThe Incas "
                         @"built the estate around 1450, but abandoned it a century later at "
                         @"the time of the Spanish Conquest. Although known locally, it was "
                         @"not known to the Spanish during the colonial period and remained "
                         @"unknown to the outside world before being brought to "
                         @"international attention in 1911 by the American historian Hiram "
                         @"Bingham. Most of the outlying buildings have been reconstructed "
                         @"in order to give tourists a better idea of what the structures "
                         @"originally looked like. By 1976, 30% of Machu Picchu had been "
                         @"restored; restoration continues today.\n\n"
                         @"Machu Picchu was declared a Peruvian Historical Sanctuary in 1981 and a "
                         @"UNESCO World Heritage Site in 1983. In 2007, Machu Picchu was voted "
                         @"one of the New Seven Wonders of the World in a worldwide Internet "
                         @"poll."];
  [_scrollView addSubview:_postambleLabel];

  // Build source attribution text view.
  NSString *sourceText = @"Source: ";
  NSMutableAttributedString *attributedText = [[NSMutableAttributedString alloc]
      initWithString:[sourceText stringByAppendingString:@"Wikipedia"]];
  [attributedText
      addAttribute:NSLinkAttributeName
             value:@"https://en.wikipedia.org/wiki/Machu_Picchu"
             range:NSMakeRange(sourceText.length, attributedText.length - sourceText.length)];

  _attributionTextView = [[UITextView alloc] init];
  _attributionTextView.editable = NO;
  _attributionTextView.attributedText = attributedText;
  _attributionTextView.font = [UIFont systemFontOfSize:16];
  [_scrollView addSubview:_attributionTextView];
  [_scrollView setAccessibilityIdentifier:@"sample_scroll_view"];
}

- (GVRPanoramaView *)getPanoramaView {
  return _panoView;
}

- (void)viewWillLayoutSubviews {
  [super viewWillLayoutSubviews];

  [self setFrameForView:_titleLabel belowView:nil margin:kMargin];
  [self setFrameForView:_subtitleLabel belowView:_titleLabel margin:kMargin];
  [self setFrameForView:_preambleLabel belowView:_subtitleLabel margin:kMargin];

  _panoView.frame = CGRectMake(kMargin,
                               CGRectGetMaxY(_preambleLabel.frame) + kMargin,
                               CGRectGetWidth(self.view.bounds) - 2 * kMargin,
                               kPanoViewHeight);
  [self setFrameForView:_captionLabel belowView:_panoView margin:kMargin];
  [self setFrameForView:_postambleLabel belowView:_captionLabel margin:kMargin];
  [self setFrameForView:_attributionTextView belowView:_postambleLabel margin:kMargin];

  _scrollView.contentSize = CGSizeMake(CGRectGetWidth(self.view.bounds),
                                       CGRectGetMaxY(_attributionTextView.frame) + kMargin);
}

#pragma mark - GVRWidgetViewDelegate

- (void)widgetView:(GVRWidgetView *)widgetView didLoadContent:(id)content {
  NSLog(@"Loaded panorama image");
}

#pragma mark - Implementation

- (UILabel *)createLabelWithFontSize:(CGFloat)fontSize text:(NSString *)text {
  return [self createLabelWithFontSize:fontSize bold:NO text:text];
}

- (UILabel *)createLabelWithFontSize:(CGFloat)fontSize bold:(BOOL)bold text:(NSString *)text {
  UILabel *label = [[UILabel alloc] init];
  label.text = text;
  label.font = (bold ? [UIFont boldSystemFontOfSize:fontSize] : [UIFont systemFontOfSize:fontSize]);
  label.numberOfLines = 0;
  return label;
}

- (void)setFrameForView:(UIView *)view belowView:(UIView *)topView margin:(CGFloat)margin {
  CGSize size =
      [view sizeThatFits:CGSizeMake(CGRectGetWidth(self.view.bounds) - 2 * kMargin, CGFLOAT_MAX)];
  view.frame = CGRectMake(kMargin, CGRectGetMaxY(topView.frame) + margin, size.width, size.height);
}

@end
