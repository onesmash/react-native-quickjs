import { NativeModules } from 'react-native';

type QuickjsType = {
  multiply(a: number, b: number): Promise<number>;
};

const { Quickjs } = NativeModules;

export default Quickjs as QuickjsType;
