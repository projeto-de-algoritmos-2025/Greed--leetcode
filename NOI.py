class Solution:
    def eraseOverlapIntervals(self, intervals: List[List[int]]) -> int:
        intervals.sort(key=lambda x: x[1])  # Ordena os intervalos pelo fim 
        
        # Inicializa o fim do primeiro intervalo como o menor possível
        end = float('-inf')
        count = 0  # contador de remoções

        for interval in intervals:
            start, finish = interval
            # Se não há sobreposição
            if start >= end:
                end = finish  # atualiza o fim do último intervalo aceito
            else:
                count += 1  # há sobreposição, remove o intervalo (conta +1)

        return count