class Solution:
    def canCompleteCircuit(self, gas: List[int], cost: List[int]) -> int:
        total = 0       # saldo total de gasolina
        tank = 0        # saldo do tanque local
        start = 0       # estação candidata a ser o ponto de partida

        for i in range(len(gas)):
            ganho = gas[i] - cost[i]    #quantidade de combustível no ponto i
            total += ganho              # soma do que ganhou e gastou
            tank += ganho               # atualiza o alcance atual

            # Se o saldo do tanque ficou negativo, começamos do próximo
            if tank < 0:
                start = i + 1
                tank = 0        # reinicia o alcance atual

        # Verifica se conseguimos completar o circuito
        return start if total >= 0 else -1