import * as React from 'react';
import { StyleSheet, View, Text } from 'react-native';
import Quickjs from 'react-native-quickjs';

export default function App() {

  return (
    <View style={styles.container}>
      <Text>Result: </Text>
    </View>
  );
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    alignItems: 'center',
    justifyContent: 'center',
  },
});
