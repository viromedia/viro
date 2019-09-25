import { Dimensions, Platform } from 'react-native';

export const isIPhoneX = () => {
  let d = Dimensions.get('window');
  const { height, width } = d;


  return (
    // This has to be iOS duh
    Platform.OS === 'ios' &&

    // Accounting for the height in either orientation
    ( (height === 812 || width === 812) || // iPhone X & XS
      (height === 896 || width === 896) )  // iPhone XR & XS Max
  );
}

export const isIOS = () => {
  return Platform.OS === 'ios';
}

export const isAndroid = () => {
  return Platform.OS === 'android';
}

export const iOSTopPadding = 20;
export const iPhoneXBottomPadding = 25;