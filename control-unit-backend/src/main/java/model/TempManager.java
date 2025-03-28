package model;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public class TempManager {
    private final List<Integer> tempList = new ArrayList<>();
    private TempState tState = TempState.NORMAL;
    private int maxVal;
    private int freq = Constants.F1;
    long startTime;
    /* from 0 to 100 */
    int openLevel = 0;

    public TempManager(final int max) {
        this.maxVal = max;
        tempList.add(100);
    }

    public List<Integer> getHistory() {
        return Collections.unmodifiableList(tempList); // Restituisce una lista non modificabile
    }

    public int getMax() {
        return Collections.max(tempList);
    }

    public int getMin() {
        return Collections.min(tempList); // Correzione: usa min() invece di max()
    }

    public int getLast() {
        return tempList.get(tempList.size() - 1); // Correzione: usa size() - 1
    }

    public TempState getTempState() {
        if (this.tState != TempState.ALLARM){
            this.updateState();
        }
        return this.tState;
    }

    /**
     * Aggiorna la lista e lo stato della temperatura.
     *
     * @param temp La temperatura da aggiungere.
     */
    public void addTemp(final int temp) {
        if (tempList.size() == maxVal) {
            tempList.remove(0);
        }
        tempList.add(temp);
    }

    public int getOpening(){
        return openLevel;
    }

    public int getFreq(){
        return this.freq;
    }
    
    public void startTimer() {
        this.startTime = System.currentTimeMillis();
    }
    
    public boolean isOver() {
        return startTime + Constants.DT_millis <= System.currentTimeMillis();
    }
    
    public void setOpening(final int opening){
        this.openLevel = opening;
    }

    public void setFreq(final int freq){
        this.freq = freq;
    }

    public void resetAllarm(){
        this.tState = TempState.NORMAL;
    }

    public void startAllarm(){
        tState = TempState.ALLARM;
    }

    private void updateState() {
        int lastTemp = getLast();
        if (lastTemp > Constants.T2 && tState != TempState.TOO_HOT) {
            //System.out.println("START TIMER");
            this.startTimer();
            this.tState = TempState.TOO_HOT;
        } else if (lastTemp > Constants.T2) {
            this.tState = TempState.TOO_HOT;
        } else if (lastTemp > Constants.T1) {
            this.tState = TempState.HOT;
        } else {
            this.tState = TempState.NORMAL;
        }
    }
}